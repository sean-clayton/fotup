open Webapi;

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

  let activeStyle = style(_active);

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

type state = {
  image: option(File.t),
  dragging: bool,
};

type action =
  | SetImage(File.t)
  | StartDragging
  | StopDragging;

let component = ReasonReact.reducerComponent("Upload");

let make = _children => {
  let handleImageSubmit = (image, {ReasonReact.send}) => {
    send(StopDragging);
    send(SetImage(image));
  };

  let handleDragEnter = (e, {ReasonReact.send}) => {
    e |> ReactEvent.Mouse.preventDefault;
    send(StartDragging);
  };

  let handleDragLeave = (e, {ReasonReact.send}) => {
    e |> ReactEvent.Mouse.preventDefault;
    send(StopDragging);
  };

  let handleDrop = (e, _self) => {
    e |> ReactEvent.Mouse.preventDefault;
  };

  let handleInputChange = (e, _self) => {
    e |> ReactEvent.Form.preventDefault;
  };

  {
    ...component,
    initialState: () => {image: None, dragging: false},
    reducer: (action, state) => {
      switch (action) {
      | SetImage(image) =>
        ReasonReact.Update({...state, image: Some(image)})
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
          let data = e->ReactEvent.Clipboard.clipboardData;
          data##files[0]->Js.log;
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
          onDrop={self.handle(handleDrop)}
          htmlFor="file">
          {"Choose a file" |> ReasonReact.string}
        </label>
      </form>;
    },
  };
};
