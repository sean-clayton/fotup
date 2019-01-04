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

  let fileInput =
    style([
      width(0.1 |> pxFloat),
      height(0.1 |> pxFloat),
      opacity(0.1),
      overflow(hidden),
      position(absolute),
      zIndex(-1),
      selector(
        ":focus + label, + label:hover",
        [backgroundColor(hex("0f0"))],
      ),
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

  let button = style([padding(0.5 |> rem)]);
};

let component = ReasonReact.statelessComponent("Upload");

let handleDragEnter = (e, _) => {
  e |> ReactEvent.Mouse.preventDefault;
  Js.log2("Drag Enter", e);
};
let handleDragLeave = (e, _) => {
  e |> ReactEvent.Mouse.preventDefault;
  Js.log2("Drag Leave", e);
};
let handleDragOver = (e, _) => {
  e |> ReactEvent.Mouse.preventDefault;
  Js.log2("Drag Over", e);
};
let handleDrop = (e, _) => {
  e |> ReactEvent.Mouse.preventDefault;
  Js.log2("Drop", e);
};
let handleInputChange = (e, _) => {
  e |> ReactEvent.Form.preventDefault;
  Js.log2("Input Change", e);
};

let handleImageSubmit = (image, _) => {
  Js.log2("Image Submit", image);
};

let make = _children => {
  ...component,
  render: self =>
    <form
      onPaste={(e: ReactEvent.Clipboard.t) => {
        let data = e->ReactEvent.Clipboard.clipboardData;
        data##files[0]->Js.log;
      }}
      className=Styles.form
      onSubmit=ReactEvent.Form.preventDefault>
      <input
        onChange={self.handle(handleInputChange)}
        className=Styles.fileInput
        id="file"
        name="file"
        type_="file"
        accept=".bmp,.gif,.jpg,.jpeg,.png,.webm,.mp4"
      />
      <label
        onDragEnter={self.handle(handleDragEnter)}
        onDragLeave={self.handle(handleDragLeave)}
        onDragOver={self.handle(handleDragOver)}
        onDrop={self.handle(handleDrop)}
        htmlFor="file">
        {"Choose a file" |> ReasonReact.string}
      </label>
      <button className=Styles.button>
        {"Upload!" |> ReasonReact.string}
      </button>
    </form>,
};
