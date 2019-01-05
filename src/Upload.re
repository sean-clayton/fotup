module Styles = {
  open Css;

  let form =
    style([
      fontSize(20 |> px),
      display(flexBox),
      flexDirection(column),
      padding2(~v=zero, ~h=2.0 |> rem),
      marginBottom(zero),
      justifyContent(center),
      textShadow(~x=zero, ~y=px(1), ~blur=px(1), rgba(0, 0, 0, 0.666)),
    ]);

  let _active = [borderStyle(dashed), backgroundColor(transparent)];

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
          backgroundColor(hex("606060")),
          border(2 |> px, solid, rgba(255, 255, 255, 0.5)),
          borderRadius(3 |> px),
          padding2(~v=3.0 |> rem, ~h=2.0 |> rem),
          marginBottom(1.0 |> rem),
        ],
      ),
    ]);

  let fileDragging = merge([fileInput, activeStyle]);
};

let component = ReasonReact.statelessComponent("Upload");

let make =
    (
      ~handleDragEnter,
      ~handleDragLeave,
      ~handleDragOver,
      ~handleDrop,
      ~handleInputChange,
      ~uploadProgress,
      ~dragging,
      _children,
    ) => {
  {
    ...component,
    render: _self => {
      let disabled = uploadProgress !== 0.0;
      let label =
        switch (disabled) {
        | true => "Upload in progress..."
        | _ => "Tap, click, or drop a file here."
        };
      let inputClassName =
        switch (dragging) {
        | true => Styles.fileDragging
        | _ => Styles.fileInput
        };
      <form className=Styles.form onSubmit=ReactEvent.Form.preventDefault>
        <input
          disabled
          onChange=handleInputChange
          className=inputClassName
          id="file"
          name="file"
          type_="file"
          accept="image/*,video/*"
        />
        <label
          onDragEnter=handleDragEnter
          onDragLeave=handleDragLeave
          onDragOver=handleDragOver
          onDrop=handleDrop
          htmlFor="file">
          {label |> ReasonReact.string}
        </label>
        <p> {"Or paste an image!" |> ReasonReact.string} </p>
      </form>;
    },
  };
};
