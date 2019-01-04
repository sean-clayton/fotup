open Webapi;
open Utils;

module Styles = {
  open Css;

  let form =
    style([
      height(100.0 |> pct),
      display(flexBox),
      flexDirection(column),
      border(px(2), dashed, hex("f00")),
      padding(1.0 |> rem),
      marginBottom(zero),
    ]);

  let _active = [color(black), backgroundColor(hex("0f0"))];

  let activeStyle = style([selector("+ label", _active)]);

  let fileInput =
    style([
      width(0.1 |> pxFloat),
      height(0.1 |> pxFloat),
      opacity(0.1),
      overflow(hidden),
      position(absolute),
      zIndex(-1),
      selector(":focus + label, + label:hover", _active),
      selector(
        "+ label",
        [
          cursor(`pointer),
          display(block),
          border(2 |> px, dashed, hex("00f")),
          padding(1.0 |> rem),
          marginBottom(1.0 |> rem),
        ],
      ),
    ]);

  let fileDragging = merge([fileInput, activeStyle]);
};

type state = {dragging: bool};

type action =
  | StartDragging
  | StopDragging;

let component = ReasonReact.reducerComponent("Upload");

let make = _children => {
  let handleFileUploaded = response => {
    redirect(response##data##link);
  };

  let uploadFile = (file: File.t) => {
    let _ =
      Js.Promise.(
        Api.uploadFile(file)
        |> then_(response =>
             switch (response##status) {
             | status when status < 400 =>
               response##data |> handleFileUploaded |> resolve
             | _ => resolve()
             }
           )
        |> catch(error => resolve(Js.log(error)))
      );
    ();
  };

  let handlePaste = (e, _self) => {
    e |> ReactEvent.Clipboard.preventDefault;
    let data =
      e
      |> ReactEvent.Clipboard.clipboardData
      |> DataTransfer.dataTransferFromJs;
    switch (data.files) {
    | [|file|] => uploadFile(file)
    | _ => ()
    };
  };

  let handleInputChange = (e, _self) => {
    e |> ReactEvent.Form.preventDefault;
    switch (e |> ReactEvent.Form.target |> Target.files) {
    | [|file|] => file |> uploadFile
    | _ => ()
    };
  };

  let handleDrop = (e, {ReasonReact.send}) => {
    e |> ReactEvent.Mouse.preventDefault;
    send(StopDragging);
    let data = e |> MouseEvent.dataTransfer |> DataTransfer.dataTransferFromJs;
    switch (data.files) {
    | [|file|] => file |> uploadFile
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

  let handleDragOver = (e, _self) => {
    e |> ReactEvent.Mouse.preventDefault;
  };

  {
    ...component,
    initialState: () => {dragging: false},
    reducer: (action, _state) => {
      switch (action) {
      | StartDragging => ReasonReact.Update({dragging: true})
      | StopDragging => ReasonReact.Update({dragging: false})
      };
    },
    render: self => {
      let inputClassName =
        switch (self.state.dragging) {
        | true => Styles.fileDragging
        | _ => Styles.fileInput
        };
      <form
        onPaste={self.handle(handlePaste)}
        className=Styles.form
        onSubmit=ReactEvent.Form.preventDefault>
        <input
          onChange={self.handle(handleInputChange)}
          className=inputClassName
          id="file"
          name="file"
          type_="file"
          accept="image/*,video/*"
        />
        <label
          onDragEnter={self.handle(handleDragEnter)}
          onDragLeave={self.handle(handleDragLeave)}
          onDragOver={self.handle(handleDragOver)}
          onDrop={self.handle(handleDrop)}
          htmlFor="file">
          {"Choose a file" |> ReasonReact.string}
        </label>
      </form>;
    },
  };
};
