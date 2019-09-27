module Styles = {
  open Css;

  let uploadWrapper =
    style([
      display(grid),
      unsafe(
        "gridTemplateAreas",
        {|"upload url"
          "upload delete-link"|},
      ),
      gridGap(1.0->rem),
      unsafe("gridTemplateRows", "min-content 1fr"),
      gridTemplateColumns([1.0->fr, 1.0->fr]),
      minHeight(16.->rem),
      height(33.33->vh),
      maxHeight(32.->rem),
      unsafe("alignItems", "start"),
      unsafe("justifyContent", "start"),
      textAlign(initial),
      flex(`num(1.0)),
      media(
        "(max-width: 480px)",
        [
          unsafe(
            "gridTemplateAreas",
            {|"upload"
              "url"
              "delete-link"|},
          ),
          unsafe("gridTemplateColumns", "1fr"),
          unsafe("gridTemplateRows", "1fr min-content min-content"),
        ],
      ),
    ]);

  let uploadFile =
    style([
      alignSelf(`initial),
      unsafe("objectFit", "contain"),
      unsafe("gridArea", "upload"),
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
      unsafe("objectFit", "contain"),
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
    style([unsafe("gridArea", "delete-link"), color("63e2ff"->hex)]);

  let input =
    style([
      unsafe("gridArea", "url"),
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

let stripLink = s =>
  s
  |> Js.String.replace(
       Utils.Environment.imageHostPath
       ->Js.Undefined.toOption
       ->Belt.Option.getExn,
       "",
     )
  |> Js.String.replace("image/", "");

let appOrigin = Webapi.Dom.(window->Window.location->Location.origin);

let origin =
  Utils.Environment.imageHostPath
  ->Js.Undefined.toOption
  ->Belt.Option.getWithDefault(appOrigin);

[@react.component]
let make = (~upload: Api.upload) => {
  <li className=Styles.listItem>
    <div className=Styles.uploadWrapper>
      {switch (upload.extension) {
       | x when x->isImageFile =>
         <Link
           className=Styles.uploadFile
           href={"/view/" ++ upload.link->stripLink}>
           <img
             className=Styles.uploadFileImage
             src={origin ++ "/" ++ upload.link->stripLink}
           />
         </Link>
       | x when x->isVideoFile =>
         <video className=Styles.uploadFile controls=true>
           <source src={origin ++ "/" ++ upload.link->stripLink} />
           {"I'm sorry; your browser doesn't support HTML5 video in WebM with VP8/VP9 or MP4 with H.264."
            |> ReasonReact.string}
         </video>
       | _ => ReasonReact.null
       }}
      <input
        className=Styles.input
        readOnly=true
        value={origin ++ "/" ++ upload.link->stripLink}
        onClick=handleCopy
      />
      <DeleteButton deleteUrl={upload.deleteLink} />
    </div>
  </li>;
};
