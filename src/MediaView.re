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

  let video = style([maxHeight(90.0->vh), maxWidth(90.0->vw)]);
};

[@react.component]
let make = (~mediaFilename, ~mediaType) => {
  let imagePath =
    Environment.imageHostPath->Js.Undefined.toOption->Belt.Option.getExn
    ++ "/"
    ++ mediaFilename;
  <div className=Styles.container>
    {switch (mediaType) {
     | Types.Media.Image =>
       <a className=Styles.imageContainer href=imagePath>
         <img className=Styles.image src=imagePath />
       </a>
     | Types.Media.Video =>
       <video className=Styles.video controls=true>
         <source src=imagePath />
         {"I'm sorry; your browser doesn't support HTML5 video in WebM with VP8/VP9 or MP4 with H.264."
          |> ReasonReact.string}
       </video>
     }}
  </div>;
};
