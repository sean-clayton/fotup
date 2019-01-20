open Webapi;
open Utils;

module Styles = {
  open Css;

  let main =
    style([
      flex(1),
      display(flexBox),
      flexDirection(column),
      justifyContent(center),
      textAlign(center),
    ]);
};

type route =
  | Home
  | ViewUpload;

type state = {
  route,
  uploading: bool,
  uploads: list(Api.upload),
  uploadProgress: float,
  dragging: bool,
  uploadFailed: bool,
};

type action =
  | StartDragging
  | StopDragging
  | StartUploading
  | FinishedUploading
  | AddFile(Api.upload)
  | UploadFailed
  | UploadProgress(float)
  | ChangeRoute(route);

let mapUrlToRoute = (url: ReasonReact.Router.url) =>
  switch (url.path) {
  | _ => Home
  };

let component = ReasonReact.reducerComponent("App");

let make = _ => {
  let handleFileUploaded = (response, {ReasonReact.send}) => {
    Api.uploadFromJs(response##data)->AddFile->send;
  };

  let handleUploadProgress = (a, {ReasonReact.send}) => {
    let e = a |> ProgressEvent.progressEventFromJs;
    let percent = e.loaded->float_of_int /. e.total->float_of_int;
    send(UploadProgress(percent));
  };

  let uploadFile =
      (
        file: File.t,
        {ReasonReact.state, ReasonReact.handle, ReasonReact.send},
      ) => {
    switch (state.uploading) {
    | false =>
      send(StartUploading);
      let _ =
        Js.Promise.(
          Api.uploadFile(
            ~onUploadProgress=handle(handleUploadProgress),
            file,
          )
          |> then_(response =>
               switch (response##status) {
               | status when status < 400 =>
                 send(FinishedUploading);
                 response##data |> handle(handleFileUploaded) |> resolve;
               | _ => resolve()
               }
             )
          |> catch(error => {
               send(UploadFailed);
               resolve(Js.log(error));
             })
        );
      ();
    | _ => ()
    };
  };

  let handlePaste = (e, {ReasonReact.handle}) => {
    e |> ReactEvent.Clipboard.preventDefault;
    let data =
      e
      |> ReactEvent.Clipboard.clipboardData
      |> DataTransfer.dataTransferFromJs;
    switch (data.files) {
    | [|file|] => file |> handle(uploadFile)
    | _ => ()
    };
  };

  let handleInputChange = (e, {ReasonReact.handle}) => {
    e |> ReactEvent.Form.preventDefault;
    switch (e |> ReactEvent.Form.target |> Target.files) {
    | [|file|] => file |> handle(uploadFile)
    | _ => ()
    };
  };

  let handleDrop = (e, {ReasonReact.send, ReasonReact.handle}) => {
    e |> ReactEvent.Mouse.preventDefault;
    send(StopDragging);
    let data = e |> MouseEvent.dataTransfer |> DataTransfer.dataTransferFromJs;
    switch (data.files) {
    | [|file|] => file |> handle(uploadFile)
    | _ => ()
    };
  };

  let handleDragEnter = (e, {ReasonReact.send}) => {
    e |> ReactEvent.Mouse.preventDefault;
    send(StartDragging);
  };

  let handleDragLeave = (e, {ReasonReact.send}) => {
    e |> ReactEvent.Mouse.preventDefault;
    send(StopDragging);
  };

  let handleDragOver = (e, {ReasonReact.send}) => {
    e |> ReactEvent.Mouse.preventDefault;
    send(StartDragging);
  };

  {
    ...component,
    initialState: () => {
      route: Home,
      uploading: false,
      uploads: [
        {
          originalName: "oaaea.mp4",
          name: "Q1New5gH.mp4",
          extension: ".mp4",
          deleteToken: "FFCegFE5",
          size: 6244472,
          thumbnailLink: None,
          link: "https://s.put.re/Q1New5gH.mp4",
          deleteLink: "https://s.put.re/delete/Q1New5gH.mp4/FFCegFE5",
        },
      ],
      uploadFailed: false,
      uploadProgress: 0.0,
      dragging: false,
    },
    reducer: (action, state) => {
      switch (action) {
      | StartDragging => ReasonReact.Update({...state, dragging: true})
      | StopDragging => ReasonReact.Update({...state, dragging: false})
      | UploadProgress(percent) =>
        ReasonReact.Update({...state, uploadProgress: percent})
      | StartUploading =>
        ReasonReact.Update({...state, uploading: true, uploadFailed: false})
      | FinishedUploading =>
        ReasonReact.Update({...state, uploading: false, uploadProgress: 0.0})
      | UploadFailed =>
        ReasonReact.Update({
          ...state,
          uploading: false,
          uploadFailed: true,
          uploadProgress: 0.0,
        })
      | ChangeRoute(route) => ReasonReact.Update({...state, route})
      | AddFile(upload) =>
        ReasonReact.Update({...state, uploads: [upload, ...state.uploads]})
      };
    },
    render: self =>
      <main
        onDragEnter={self.handle(handleDragEnter)}
        onDragLeave={self.handle(handleDragLeave)}
        onDragOver={self.handle(handleDragOver)}
        onDrop={self.handle(handleDrop)}
        onPaste={self.handle(handlePaste)}
        className=Styles.main>
        <Logo />
        <UploadForm
          uploading={self.state.uploading}
          uploadFailed={self.state.uploadFailed}
          uploadProgress={self.state.uploadProgress}
          dragging={self.state.dragging}
          handleInputChange={self.handle(handleInputChange)}
        />
        <Uploads uploads={self.state.uploads} />
        <Footer />
      </main>,
  };
};
