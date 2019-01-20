module Styles = {
  open Css;
  open Utils.Styling;

  let uploadWrapper =
    style([
      display(grid),
      property(
        "grid-template-areas",
        {|"upload url"
          "upload delete-link"|},
      ),
      gridGap(1.0->rem),
      property("grid-template-rows", "min-content 1fr"),
      gridTemplateColumns([1.0->fr, 1.0->fr]),
      minHeight(16.->rem),
      height(33.33->vh),
      maxHeight(32.->rem),
      property("align-items", "start"),
      property("justify-content", "start"),
      textAlign(initial),
      flex(1),
      media(
        "(max-width: 480px)",
        [
          property(
            "grid-template-areas",
            {|"upload"
              "url"
              "delete-link"|},
          ),
          property("grid-template-columns", "1fr"),
          property("grid-template-rows", "1fr min-content min-content"),
        ],
      ),
    ]);

  let uploadFile =
    style([
      alignSelf(stretch),
      property("object-fit", "contain"),
      property("grid-area", "upload"),
      display(block),
      boxSizing(borderBox),
      height(100.0->pct),
      width(100.0->pct),
      margin(zero),
      backgroundColor(rgba(0, 0, 0, 0.333)),
      padding(0.25->rem),
      borderRadius(3->px),
    ]);

  let listItem =
    style([
      display(flexBox),
      flex(1),
      padding2(~v=1.0->rem, ~h=zero),
      marginBottom(1.0->rem),
      listStyleType(none),
      borderBottom(1->px, solid, rgba(0, 0, 0, 0.5)),
      selector(
        ":last-of-type",
        [
          marginBottom(zero),
          padding3(~top=0.0->rem, ~h=zero, ~bottom=0.0->rem),
          borderBottom(0->px, none, transparent),
        ],
      ),
      media(
        "(min-width: 480px)",
        [marginBottom(zero), borderBottom(0->px, none, transparent)],
      ),
    ]);

  let infoContainer =
    style([
      flex(1),
      display(flexBox),
      flexDirection(column),
      marginLeft(1.0->rem),
      textAlign(`left),
    ]);

  let infoLink =
    style([property("grid-area", "delete-link"), color("63e2ff"->hex)]);

  let input =
    style([
      property("grid-area", "url"),
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

let handleCopy = (e, _self) => {
  e->ReactEvent.Mouse.preventDefault;
  e->ReactEvent.Mouse.persist;
  let target = e->ReactEvent.Mouse.target;
  let _ = target##select();
  %raw
  {|document.execCommand("copy")|};
};

let make = (~upload: Api.upload, _children) => {
  ...component,
  render: self =>
    <li className=Styles.listItem>
      <div className=Styles.uploadWrapper>
        {switch (upload.extension) {
         | x when x->isImageFile =>
           <img className=Styles.uploadFile src={upload.link} />
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
          onClick={self.handle(handleCopy)}
        />
        <a className=Styles.infoLink href={upload.deleteLink}>
          {"Delete" |> ReasonReact.string}
        </a>
      </div>
    </li>,
};
