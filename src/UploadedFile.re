module Styles = {
  open Css;

  let uploadWrapper =
    style([
      display(grid),
      unsafe(
        "gridTemplateAreas",
        {|"upload url url url"
          "upload delete-link cancel-link ."|},
      ),
      gridGap(1.0->rem),
      unsafe("gridTemplateRows", "min-content 1fr"),
      gridTemplateColumns([1.0->fr, 8.0->rem, 8.0->rem, 1.0->fr]),
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
              "delete-link"
              "cancel-link"|},
          ),
          unsafe("gridTemplateColumns", "1fr"),
          unsafe(
            "gridTemplateRows",
            "1fr min-content min-content min-content",
          ),
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
    style([
      unsafe("gridArea", "delete-link"),
      color("63e2ff"->hex),
      background(rgba(0, 0, 0, 0.333)),
      fontSize(1.0->em),
      border(zero, none, transparent),
      cursor(`pointer),
      padding(0.5->rem),
      borderRadius(3->px),
    ]);

  let cancelLink =
    merge([infoLink, style([unsafe("gridArea", "cancel-link")])]);

  let confirmLink =
    merge([infoLink, style([background("f11"->hex), color(white)])]);

  let input =
    style([
      unsafe("gridArea", "url"),
      fontSize(1.0->rem),
      padding(0.5->rem),
      borderRadius(3->px),
      border(zero, none, transparent),
      display(block),
      cursor(`pointer),
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
  type state =
    | NotAsked
    | Asked;

  [@react.component]
  let make = (~upload, ~removeFile) => {
    let (state, setState) = React.useState(() => NotAsked);

    let className =
      switch (state) {
      | NotAsked => Styles.infoLink
      | Asked => Styles.confirmLink
      };

    let handleCancelRemove = e => {
      e->ReactEvent.Mouse.preventDefault;
      setState(_ => NotAsked);
    };

    let handleClick = e => {
      e->ReactEvent.Mouse.preventDefault;
      switch (state) {
      | NotAsked => setState(_ => Asked)
      | Asked =>
        removeFile(upload);
        ();
      };
    };

    <>
      <button className onClick=handleClick>
        {switch (state) {
         | NotAsked => "Delete file?"->ReasonReact.string
         | Asked => "Delete file"->ReasonReact.string
         }}
      </button>
      {switch (state) {
       | NotAsked => React.null
       | Asked =>
         <button className=Styles.cancelLink onClick=handleCancelRemove>
           "Cancel"->React.string
         </button>
       }}
    </>;
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
let make = (~upload: Api.upload, ~removeFile) => {
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
      <DeleteButton upload removeFile />
    </div>
  </li>;
};
