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

type maybeFile = option(File.t);

type state = {
  image: maybeFile,
  dragging: bool,
};

type action =
  | SetImage(maybeFile)
  | StartDragging
  | StopDragging;

let component = ReasonReact.reducerComponent("Upload");

let make = _children => {
  let handleFileUploaded = response => {
    let _ = redirect(response##data##link);
    ();
  };

  let uploadFile = (file: File.t) => {
    let _ =
      Js.Promise.(
        Api.uploadFile(file)
        |> then_(response =>
             switch (response##status) {
             | status when status < 400 =>
               let data = response##data;
               let _ = handleFileUploaded(data);
               resolve();
             | _ => resolve()
             }
           )
        |> catch(error => resolve(Js.log(error)))
      );
    ();
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

  let handleDrop = (e, {ReasonReact.send}) => {
    e |> ReactEvent.Mouse.preventDefault;
    send(StopDragging);
    let file: maybeFile = [%raw "e.dataTransfer.files[0]"];
    file |> Js.log2("File");
  };

  let handleInputChange = (e, _self) => {
    e |> ReactEvent.Form.preventDefault;
    e |> Js.log2("InputChange");
  };

  {
    ...component,
    initialState: () => {image: None, dragging: false},
    reducer: (action, state) => {
      switch (action) {
      | SetImage(maybeFile) =>
        switch (maybeFile) {
        | Some(file) => ReasonReact.Update({...state, image: Some(file)})
        | _ => ReasonReact.NoUpdate
        }
      | StartDragging => ReasonReact.Update({...state, dragging: true})
      | StopDragging => ReasonReact.Update({...state, dragging: false})
      };
    },
    render: self => {
      let inputClassName =
        switch (self.state.dragging) {
        | true => Styles.fileDragging
        | _ => Styles.fileInput
        };
      <form
        onPaste={(e: ReactEvent.Clipboard.t) => {
          e |> ReactEvent.Clipboard.persist;
          e |> ReactEvent.Clipboard.preventDefault;
          let data =
            e
            |> ReactEvent.Clipboard.clipboardData
            |> DataTransfer.dataTransferFromJs;
          switch (data.files) {
          | [|file|] => uploadFile(file)
          | _ => ()
          };
        }}
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
