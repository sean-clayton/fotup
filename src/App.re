module Styles = {
  open Css;

  let main = style([flex(1)]);
};

let component = ReasonReact.statelessComponent("App");

let make = _ => {
  ...component,
  render: _ => <main className=Styles.main> <Upload /> </main>,
};
