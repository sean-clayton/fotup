open Utils;

module Styles = {
  open Css;
  let imageContainer =
    style([
      unsafe("objectFit", "contain"),
      cursor(`zoomIn),
      display(block),
      top(10.0->pct),
      left(10.0->pct),
      right(10.0->pct),
      bottom(10.0->pct),
    ]);

  let container =
    style([
      display(`flex),
      justifyContent(center),
      alignItems(center),
      maxHeight(100.0->vh),
      maxWidth(100.0->vw),
    ]);

  let image = style([maxHeight(90.0->vh), maxWidth(90.0->vw)]);
};

[@react.component]
let make = (~imageFilename) => {
  let imagePath =
    Environment.imageHostPath->Js.Undefined.toOption->Belt.Option.getExn
    ++ "/"
    ++ imageFilename;
  <div className=Styles.container>
    <a className=Styles.imageContainer href=imagePath>
      <img className=Styles.image src=imagePath />
    </a>
  </div>;
};
