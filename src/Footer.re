open Utils;

module Styles = {
  open Css;

  let footer =
    style([
      marginTop(2.0->rem),
      paddingBottom(2.0->rem),
      flex(1),
      display(flexBox),
      flexDirection(column),
      justifyContent(flexEnd),
      color(rgb(160, 160, 160)),
    ]);

  let p = style([margin2(~v=0.25->rem, ~h=zero), fontSize(0.8->rem)]);

  let link =
    style([
      fontWeight(extraBold),
      color(hex("63e2ff")),
      textDecoration(none),
    ]);
};

let component = ReasonReact.statelessComponent("Footer");

let date = Js.Date.make()->Js.Date.getFullYear->int_of_float->string_of_int;

let make = _children => {
  {
    ...component,
    render: _self => {
      let buildId =
        switch (Js.Undefined.toOption(Environment.commitRef)) {
        | None => "DEV"
        | Some(buildId) => buildId
        };

      <footer className=Styles.footer>
        <About />
        <p className=Styles.p>
          {{js|Made with 🥃 in Louisville, KY|js} |> ReasonReact.string}
        </p>
        <p className=Styles.p>
          {"Build ID: " |> ReasonReact.string}
          {switch (buildId) {
           | "DEV" => "DEV" |> ReasonReact.string
           | buildId =>
             <a
               className=Styles.link
               href={
                 "https://github.com/sean-clayton/fotup/commit/" ++ buildId
               }>
               {buildId |> ReasonReact.string}
             </a>
           }}
        </p>
        <p className=Styles.p>
          {"Copyright " ++ date |> ReasonReact.string}
        </p>
      </footer>;
    },
  };
};
