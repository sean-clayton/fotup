open Utils;
open React;

module File = Webapi.File;

module Styles = {
  open Css;

  let main =
    style([
      flex(`num(1.0)),
      display(flexBox),
      flexDirection(column),
      justifyContent(center),
      textAlign(center),
    ]);
};

[@bs.deriving jsConverter]
type state = {
  uploading: bool,
  uploads: list(Api.upload),
  uploadProgress: float,
  dragging: bool,
  uploadFailed: bool,
};

type action =
  | Initialize
  | StartDragging
  | StopDragging
  | StartUploading
  | FinishedUploading
  | AddFile(Api.upload)
  | UploadFailed
  | UploadProgress(float);

let serializeUploads = (uploads: list(Api.upload)) => {
  let serialized =
    uploads
    ->Belt.List.map(upload => Api.uploadToJs(upload))
    ->Belt.List.toArray
    ->Js.Json.stringifyAny
    ->Belt.Option.getWithDefault("");
  Dom.Storage.localStorage
  |> Dom.Storage.setItem("__FOTUP__UPLOADS__", serialized);
};

let deserializeUploads = {
  open Belt;
  open Js.Json;

  let serialized =
    (Dom.Storage.localStorage |> Dom.Storage.getItem("__FOTUP__UPLOADS__"))
    ->Option.getWithDefault("[]");

  serialized
  ->parseExn
  ->decodeArray
  ->Option.getWithDefault([||])
  ->Array.map(decodeObject)
  ->Array.map(maybeObj =>
      Option.flatMap(
        maybeObj,
        dict => {
          let keys = (
            dict->Js.Dict.get("originalName")->Option.flatMap(decodeString),
            dict->Js.Dict.get("name")->Option.flatMap(decodeString),
            dict->Js.Dict.get("extension")->Option.flatMap(decodeString),
            dict->Js.Dict.get("deleteToken")->Option.flatMap(decodeString),
            dict
            ->Js.Dict.get("size")
            ->Option.flatMap(decodeNumber)
            ->Option.map(int_of_float),
            dict->Js.Dict.get("thumbnailLink")->Option.flatMap(decodeString),
            dict->Js.Dict.get("link")->Option.flatMap(decodeString),
            dict->Js.Dict.get("deleteLink")->Option.flatMap(decodeString),
          );
          switch (keys) {
          | (
              Some(originalName),
              Some(name),
              Some(extension),
              Some(deleteToken),
              Some(size),
              thumbnailLink,
              Some(link),
              Some(deleteLink),
            ) =>
            Some(
              Api.uploadFromJs({
                "originalName": originalName,
                "name": name,
                "extension": extension,
                "deleteToken": deleteToken,
                "size": size,
                "thumbnailLink": thumbnailLink,
                "link": link,
                "deleteLink": deleteLink,
              }),
            )
          | _ => None
          };
        },
      )
    )
  ->Array.keep(Option.isSome)
  ->Array.map(Option.getExn);
};

[@react.component]
let make = () => {
  let url = ReasonReactRouter.useUrl();

  let (state, dispatch) =
    useReducer(
      (state, action) =>
        switch (action) {
        | Initialize =>
          let uploads = deserializeUploads->Belt.List.fromArray;
          {...state, uploads};
        | StartDragging => {...state, dragging: true}
        | StopDragging => {...state, dragging: false}
        | UploadProgress(percent) => {...state, uploadProgress: percent}
        | StartUploading => {...state, uploading: true, uploadFailed: false}
        | FinishedUploading => {
            ...state,
            uploading: false,
            uploadProgress: 0.0,
          }
        | UploadFailed => {
            ...state,
            uploading: false,
            uploadFailed: true,
            uploadProgress: 0.0,
          }
        | AddFile(upload) =>
          let newState = {...state, uploads: [upload, ...state.uploads]};
          serializeUploads(newState.uploads);
          newState;
        },
      {
        uploading: false,
        uploads: [],
        uploadFailed: false,
        uploadProgress: 0.0,
        dragging: false,
      },
    );

  useEffect1(
    () => {
      dispatch(Initialize);
      None;
    },
    [||],
  );

  let transformUpload = upload => {
    open Api;
    open Js;
    let uploadLink = String.make(upload.link);
    {
      ...upload,
      link:
        uploadLink
        |> String.replace(
             "https://s.put.re/",
             Environment.imageHostPath
             ->Js.Undefined.toOption
             ->Belt.Option.getExn
             ->String.make,
           ),
    };
  };

  let handleFileUploaded = response => {
    Api.uploadFromJs(response##data)->transformUpload->AddFile->dispatch;
  };

  let handleUploadProgress = a => {
    let e = a->ProgressEvent.progressEventFromJs;
    let percent = e.loaded->float_of_int /. e.total->float_of_int;
    dispatch(UploadProgress(percent));
  };

  let uploadFile = (file: File.t) => {
    switch (state.uploading) {
    | false =>
      dispatch(StartUploading);
      let _ =
        Js.Promise.(
          Api.uploadFile(~onUploadProgress=handleUploadProgress, file)
          |> then_(response =>
               switch (response##status) {
               | status when status < 400 =>
                 dispatch(FinishedUploading);
                 response##data->handleFileUploaded->resolve;
               | _ => resolve()
               }
             )
          |> catch(error => {
               dispatch(UploadFailed);
               resolve(Js.log(error));
             })
        );
      ();
    | _ => ()
    };
  };

  let handlePaste = e => {
    e->ReactEvent.Clipboard.preventDefault;
    let data =
      e->ReactEvent.Clipboard.clipboardData->DataTransfer.dataTransferFromJs;
    switch (data.files) {
    | [|file|] => file->uploadFile
    | _ => ()
    };
  };

  let handleInputChange = e => {
    e->ReactEvent.Form.preventDefault;
    switch (e->ReactEvent.Form.target->Target.files) {
    | [|file|] => file->uploadFile
    | _ => ()
    };
  };

  let handleDrop = e => {
    e->ReactEvent.Mouse.preventDefault;
    dispatch(StopDragging);
    let data = e->MouseEvent.dataTransfer->DataTransfer.dataTransferFromJs;
    switch (data.files) {
    | [|file|] => file->uploadFile
    | _ => ()
    };
  };

  let handleDragEnter = e => {
    e->ReactEvent.Mouse.preventDefault;
    dispatch(StartDragging);
  };

  let handleDragLeave = e => {
    e->ReactEvent.Mouse.preventDefault;
    dispatch(StopDragging);
  };

  let handleDragOver = e => {
    e->ReactEvent.Mouse.preventDefault;
    dispatch(StartDragging);
  };

  switch (url.path) {
  | ["view", mediaFilename, ..._] =>
    <main className=Styles.main>
      <MediaView mediaFilename mediaType=Types.Media.Image />
    </main>
  | ["player", mediaFilename, ..._] =>
    <main className=Styles.main>
      <MediaView mediaFilename mediaType=Types.Media.Video />
    </main>
  | _ =>
    <main
      onDragEnter=handleDragEnter
      onDragLeave=handleDragLeave
      onDragOver=handleDragOver
      onDrop=handleDrop
      onPaste=handlePaste
      className=Styles.main>
      <Logo />
      <UploadForm
        uploading={state.uploading}
        uploadFailed={state.uploadFailed}
        uploadProgress={state.uploadProgress}
        dragging={state.dragging}
        handleInputChange
      />
      <Uploads uploads={state.uploads} />
      <Footer />
    </main>
  };
};
