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

type state = {
  uploadProgress: float,
  dragging: bool,
};

type action =
  | StartDragging
  | StopDragging
  | StartUploading
  | CompleteUploading
  | UploadProgress(float);

let component = ReasonReact.reducerComponent("App");

let make = _ => {
  let handleFileUploaded = response => {
    redirect(response##data##link);
    Js.log(response);
  };

  let handleUploadProgress = (a, _self) => {
    Js.log(a);
  };

  let uploadFile = (file: File.t, {ReasonReact.handle, ReasonReact.send}) => {
    send(StartUploading);
    let _ =
      Js.Promise.(
        Api.uploadFile(~onUploadProgress=handle(handleUploadProgress), file)
        |> then_(response =>
             switch (response##status) {
             | status when status < 400 =>
               send(CompleteUploading);
               response##data |> handleFileUploaded |> resolve;
             | _ => resolve()
             }
           )
        |> catch(error => resolve(Js.log(error)))
      );
    ();
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
    initialState: () => {uploadProgress: 0.0, dragging: false},
    reducer: (action, state) => {
      switch (action) {
      | StartDragging => ReasonReact.Update({...state, dragging: true})
      | StopDragging => ReasonReact.Update({...state, dragging: false})
      | UploadProgress(percent) =>
        ReasonReact.Update({...state, uploadProgress: percent})
      | StartUploading => ReasonReact.Update({...state, uploadProgress: 0.5})
      | CompleteUploading =>
        ReasonReact.Update({...state, uploadProgress: 0.0})
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
        <Upload
          uploadProgress={self.state.uploadProgress}
          dragging={self.state.dragging}
          handleInputChange={self.handle(handleInputChange)}
        />
        <Footer />
      </main>,
  };
};
