module Styles = {
  open Css;

  let article =
    style([
      color(rgba(255, 255, 255, 0.75)),
      textShadow(~x=zero, ~y=px(1), ~blur=px(1), rgba(0, 0, 0, 0.333)),
    ]);

  let link =
    style([
      fontWeight(extraBold),
      color(hex("63e2ff")),
      textDecoration(none),
    ]);

  let attribution =
    style([
      fontSize(px(12)),
      color(rgba(255, 255, 255, 0.2)),
      fontWeight(extraLight),
      textShadow(~x=zero, ~y=zero, ~blur=zero, transparent),
    ]);
};

let component = ReasonReact.statelessComponent("About");

let make = _children => {
  ...component,
  render: _self =>
    <article className=Styles.article>
      <p> {"App made by S. P. O. Clayton" |> ReasonReact.string} </p>
      <p>
        <a className=Styles.link href="https://github.com/sean-clayton/fotup">
          {"Source Code" |> ReasonReact.string}
        </a>
      </p>
      <p className=Styles.attribution>
        {"Icon made by Becris from www.flaticon.com is licensed by CC 3.0 BY"
         |> ReasonReact.string}
      </p>
    </article>,
};
