[@bs.module] external logo: string = "./assets/fotup-logo.svg";

module Styles = {
  open Css;

  let header =
    style([
      flex(`num(1.0)),
      display(flexBox),
      justifyContent(center),
      alignItems(flexStart),
      marginBottom(2.0->rem),
    ]);

  let logo =
    style([
      display(flexBox),
      justifyContent(center),
      alignItems(center),
      fontSize(3.0 |> em),
    ]);

  let image = style([maxHeight(3.0 |> rem), marginRight(1.0 |> rem)]);

  let text = style([textTransform(uppercase), fontWeight(extraBold)]);
};

[@react.component]
let make = () =>
  <header className=Styles.header>
    <h1 className=Styles.logo>
      <img alt="Fotup Logo" className=Styles.image src=logo />
      <span className=Styles.text> {"Fotup" |> ReasonReact.string} </span>
    </h1>
  </header>;
