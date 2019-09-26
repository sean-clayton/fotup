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

let serializeState = () => {
  let _ = Dom.Storage.getItem;
  ();
};

let deserializeState = stateFromJs;

[@react.component]
let make = () => {
  let url = ReasonReactRouter.useUrl();

  let (state, dispatch) =
    useReducer(
      (state, action) =>
        switch (action) {
        | Initialize =>
          Js.log("Initializing app");
          state;
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
        | AddFile(upload) => {...state, uploads: [upload, ...state.uploads]}
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
  | ["image", imageFilename, ..._] =>
    <main className=Styles.main> <ImageView imageFilename /> </main>
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
