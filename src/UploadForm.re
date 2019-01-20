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

  let active = [borderStyle(dashed), backgroundColor(transparent)];

  let disabled = [
    pointerEvents(none),
    backgroundColor(rgba(0, 0, 0, 0.25)),
  ];

  let activeStyle = style([selector("+ label", active)]);

  let fileInput =
    style([
      width(0.1 |> pxFloat),
      height(0.1 |> pxFloat),
      opacity(0.1),
      overflow(hidden),
      position(absolute),
      zIndex(-1),
      selector(":focus + label, + label:hover", active),
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
      selector(":disabled + label", disabled),
    ]);

  let label = style([zIndex(1)]);

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
    (
      ~handleInputChange,
      ~uploading,
      ~uploadProgress,
      ~dragging,
      ~uploadFailed,
      _children,
    ) => {
  {
    ...component,
    render: _self => {
      let disabled = uploading == true;
      let progress =
        (uploadProgress *. 100.0)->Js.Float.toFixedWithPrecision(~digits=2);
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
          {switch (uploading) {
           | true => <span style=progressStyle className=Styles.progress />
           | _ => ReasonReact.null
           }}
          <span className=Styles.label> {label |> ReasonReact.string} </span>
        </label>
        <p> {"Or paste an image!" |> ReasonReact.string} </p>
      </form>;
    },
  };
};
