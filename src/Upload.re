module Styles = {
  open Css;

  let form =
    style([
      flex(3),
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
          position(relative),
          cursor(`pointer),
          display(flexBox),
          alignItems(center),
          justifyContent(center),
          backgroundColor(hex("606060")),
          border(2 |> px, solid, rgba(255, 255, 255, 0.5)),
          borderRadius(3 |> px),
          height(10.0 |> rem),
          marginBottom(1.0 |> rem),
        ],
      ),
    ]);

  let progress =
    style([
      display(flexBox),
      alignItems(center),
      justifyContent(center),
      position(absolute),
      top(zero),
      left(zero),
      right(zero),
      bottom(zero),
      backgroundColor(rgba(0, 255, 0, 0.25)),
      zIndex(0),
      transition(~duration=100, ~timingFunction=easeInOut, "transform"),
    ]);

  let fileDragging = merge([fileInput, activeStyle]);
};

let component = ReasonReact.statelessComponent("Upload");

let make =
    (~handleInputChange, ~uploadProgress, ~dragging, ~uploadFailed, _children) => {
  {
    ...component,
    render: _self => {
      let disabled = uploadProgress !== 0.0;
      let progress = (uploadProgress *. 100.0)->int_of_float->string_of_int;
      let progressStyle =
        ReactDOMRe.Style.make(
          ~transform="scaleX(" ++ uploadProgress->Js.Float.toString ++ ")",
          ~transformOrigin="left",
          (),
        );
      let label =
        switch (disabled, uploadFailed) {
        | (true, false) => "Upload in progress... (" ++ progress ++ "%)"
        | (true, true) => "Upload failed!"
        | (false, _) => "Tap, click, or drop a file here."
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
        <label htmlFor="file">
          {switch (uploadProgress) {
           | x when x > 0.0 =>
             <span style=progressStyle className=Styles.progress />
           | _ => ReasonReact.null
           }}
          <span> {label |> ReasonReact.string} </span>
        </label>
        <p> {"Or paste an image!" |> ReasonReact.string} </p>
      </form>;
    },
  };
};
