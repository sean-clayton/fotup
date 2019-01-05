[@bs.module] external logo: string = "./assets/fotup-logo.svg";

module Styles = {
  open Css;

  let logo =
    style([
      display(flexBox),
      justifyContent(center),
      textAlign(center),
      alignItems(center),
      fontSize(3.0 |> em),
      margin3(~top=2.0 |> rem, ~bottom=2.0 |> rem, ~h=zero),
    ]);

  let image = style([maxHeight(3.0 |> rem), marginRight(1.0 |> rem)]);

  let text = style([textTransform(uppercase), fontWeight(extraBold)]);
};

let component = ReasonReact.statelessComponent("Logo");

let make = _children => {
  ...component,
  render: _self =>
    <h1 className=Styles.logo>
      <img className=Styles.image src=logo />
      <span className=Styles.text> {"Fotup" |> ReasonReact.string} </span>
    </h1>,
};
