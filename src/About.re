module Styles = {
  open Css;

  let article =
    style([
      color(rgba(255, 255, 255, 0.75)),
      textShadow(
        Shadow.text(~x=zero, ~y=px(1), ~blur=px(1), rgba(0, 0, 0, 0.333)),
      ),
    ]);

  let p = style([display(block)]);

  let link =
    style([
      display(block),
      margin2(~v=0.5->rem, ~h=zero),
      fontWeight(extraBold),
      color(hex("63e2ff")),
      textDecoration(none),
    ]);

  let attribution =
    merge([
      p,
      style([
        fontSize(px(12)),
        color(rgba(255, 255, 255, 0.5)),
        fontWeight(extraLight),
        textShadow(Shadow.text(~x=zero, ~y=zero, ~blur=zero, transparent)),
      ]),
    ]);
};

[@react.component]
let make = () => {
  <article className=Styles.article>
    <span className=Styles.p>
      {"App made by S. P. O. Clayton" |> ReasonReact.string}
    </span>
    <span className=Styles.p>
      <a className=Styles.link href="https://github.com/sean-clayton/fotup">
        {"Source Code" |> ReasonReact.string}
      </a>
    </span>
    <span className=Styles.attribution>
      {"Icon made by Becris from www.flaticon.com is licensed by CC 3.0 BY"
       |> ReasonReact.string}
    </span>
  </article>;
};
