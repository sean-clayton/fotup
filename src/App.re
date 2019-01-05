module Styles = {
  open Css;

  let main =
    style([
      flex(1),
      display(flexBox),
      flexDirection(column),
      justifyContent(center),
    ]);
};

let component = ReasonReact.statelessComponent("App");

let make = _ => {
  ...component,
  render: _ => <main className=Styles.main> <Logo /> <Upload /> </main>,
};
