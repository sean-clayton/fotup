module Styles = {
  open Css;

  let uploadWrapper = style([display(flexBox), flex(1)]);

  let uploadFile =
    style([
      alignSelf(flexStart),
      display(block),
      boxSizing(contentBox),
      height(100.0->pct),
      width(8.0->rem),
      margin(zero),
      backgroundColor(rgba(0, 0, 0, 0.333)),
      padding(0.25->rem),
      borderRadius(3->px),
    ]);

  let listItem =
    style([
      display(flexBox),
      flex(1),
      selector(":last-of-type", [marginBottom(zero)]),
      padding(zero),
      margin3(~top=zero, ~h=zero, ~bottom=2.0->rem),
      listStyleType(none),
    ]);

  let infoContainer =
    style([
      flex(1),
      display(flexBox),
      flexDirection(column),
      marginLeft(1.0->rem),
      textAlign(`left),
    ]);

  let infoLink = style([color("63e2ff"->hex), alignSelf(flexStart)]);

  let input =
    style([
      fontSize(1.0->rem),
      padding(0.5->rem),
      border(zero, none, transparent),
      display(block),
      color(white),
      background(rgba(0, 0, 0, 0.333)),
      marginBottom(1.0->rem),
    ]);
};

let imageExtensions = [".jpg", ".jpeg", ".gif", ".png", ".webp"];
let videoExtensions = [".mp4", ".webm"];

let isImageFile = x => List.exists(y => y == x, imageExtensions);
let isVideoFile = x => List.exists(y => y == x, videoExtensions);

let extensionToMimetype = x =>
  switch (x) {
  | ".jpg" => "image/jpg"
  | ".jpeg" => "image/jpeg"
  | ".gif" => "image/gif"
  | ".png" => "image/png"
  | ".webp" => "image/webp"
  | ".mp4" => "video/mp4"
  | ".webm" => "video/webm"
  | _ => "application/octet-stream"
  };

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
        <section className=Styles.infoContainer>
          <input
            className=Styles.input
            readOnly=true
            value={upload.link}
            onClick={self.handle(handleCopy)}
          />
          <a className=Styles.infoLink href={upload.deleteLink}>
            {"Delete" |> ReasonReact.string}
          </a>
        </section>
      </div>
    </li>,
};
