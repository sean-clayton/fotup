module Styles = {
  open Css;

  let footer = style([flex(1)]);

  let copyright = style([color(rgba(255, 255, 255, 0.5))]);

  let loveCopy = style([color(rgb(160, 160, 160))]);
};

let component = ReasonReact.statelessComponent("Footer");

let date = Js.Date.make()->Js.Date.getFullYear->int_of_float->string_of_int;

let make = _children => {
  {
    ...component,
    render: _self =>
      <footer className=Styles.footer>
        <About />
        <p className=Styles.loveCopy>
          {{js|Made with 🥃 in Louisville, KY|js} |> ReasonReact.string}
        </p>
        <p className=Styles.copyright>
          {"Copyright " ++ date |> ReasonReact.string}
        </p>
      </footer>,
  };
};
