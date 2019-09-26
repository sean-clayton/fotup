module Styles = {
  open Css;

  let uploadWrapper =
    style([
      display(grid),
      unsafe(
        "grid-template-areas",
        {|"upload url"
          "upload delete-link"|},
      ),
      gridGap(1.0->rem),
      unsafe("grid-template-rows", "min-content 1fr"),
      gridTemplateColumns([1.0->fr, 1.0->fr]),
      minHeight(16.->rem),
      height(33.33->vh),
      maxHeight(32.->rem),
      unsafe("align-items", "start"),
      unsafe("justify-content", "start"),
      textAlign(initial),
      flex(`num(1.0)),
      media(
        "(max-width: 480px)",
        [
          unsafe(
            "grid-template-areas",
            {|"upload"
              "url"
              "delete-link"|},
          ),
          unsafe("grid-template-columns", "1fr"),
          unsafe("grid-template-rows", "1fr min-content min-content"),
        ],
      ),
    ]);

  let uploadFile =
    style([
      alignSelf(`initial),
      unsafe("object-fit", "contain"),
      unsafe("grid-area", "upload"),
      display(block),
      boxSizing(borderBox),
      height(100.0->pct),
      width(100.0->pct),
      margin(zero),
      backgroundColor(rgba(0, 0, 0, 0.333)),
      padding(0.25->rem),
      borderRadius(3->px),
    ]);

  let uploadFileImage =
    style([
      unsafe("object-fit", "contain"),
      display(block),
      height(100.0->pct),
      width(100.0->pct),
    ]);

  let listItem =
    style([
      display(flexBox),
      flex(`num(1.0)),
      padding2(~v=1.0->rem, ~h=zero),
      marginBottom(1.0->rem),
      listStyleType(none),
      borderBottom(1->px, solid, rgba(0, 0, 0, 0.5)),
      lastOfType([
        marginBottom(zero),
        padding3(~top=0.0->rem, ~h=zero, ~bottom=0.0->rem),
        borderBottom(0->px, none, transparent),
      ]),
      media(
        "(min-width: 480px)",
        [marginBottom(zero), borderBottom(0->px, none, transparent)],
      ),
    ]);

  let infoContainer =
    style([
      flex(`num(1.0)),
      display(flexBox),
      flexDirection(column),
      marginLeft(1.0->rem),
      textAlign(`left),
    ]);

  let infoLink =
    style([unsafe("grid-area", "delete-link"), color("63e2ff"->hex)]);

  let input =
    style([
      unsafe("grid-area", "url"),
      fontSize(1.0->rem),
      padding(0.5->rem),
      border(zero, none, transparent),
      display(block),
      color(white),
      background(rgba(0, 0, 0, 0.333)),
    ]);
};

let imageExtensions = [".jpg", ".jpeg", ".gif", ".png", ".webp"];
let videoExtensions = [".mp4", ".webm"];

let isImageFile = x => List.exists(y => y == x, imageExtensions);
let isVideoFile = x => List.exists(y => y == x, videoExtensions);

let component = ReasonReact.statelessComponent("UploadedFile");

let handleCopy = e => {
  e->ReactEvent.Mouse.preventDefault;
  e->ReactEvent.Mouse.persist;
  let target = e->ReactEvent.Mouse.target;
  let _ = target##select();
  %raw
  {|document.execCommand("copy")|};
};

module DeleteButton = {
  [@react.component]
  let make = (~deleteUrl) => {
    <a className=Styles.infoLink href=deleteUrl>
      {"Delete" |> ReasonReact.string}
    </a>;
  };
};

let stripLink = Js.String.replace("https://s.put.re/", "");

[@react.component]
let make = (~upload: Api.upload) => {
  <li className=Styles.listItem>
    <div className=Styles.uploadWrapper>
      {switch (upload.extension) {
       | x when x->isImageFile =>
         <Link
           className=Styles.uploadFile
           href={
             [|"image", upload.link->stripLink|] |> Js.Array.joinWith("/")
           }>
           <img className=Styles.uploadFileImage src={upload.link} />
         </Link>
       | x when x->isVideoFile =>
         <video className=Styles.uploadFile controls=true>
           <source src={upload.link} />
           {"I'm sorry; your browser doesn't support HTML5 video in WebM with VP8/VP9 or MP4 with H.264."
            |> ReasonReact.string}
         </video>
       | _ => ReasonReact.null
       }}
      <input
        className=Styles.input
        readOnly=true
        value={upload.link}
        onClick=handleCopy
      />
      <DeleteButton deleteUrl={upload.deleteLink} />
    </div>
  </li>;
};
