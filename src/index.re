module Styles = {
  open Css;

  let globalStyles = () => {
    global("*", [boxSizing(borderBox)]);
    global("html, body", [padding(zero), margin(zero)]);
    global(
      "body",
      [
        fontFamily(
          "system-ui, -apple-system, BlinkMacSystemFont, \"Segoe UI\", \"Roboto\", \"Oxygen\", \"Ubuntu\", \"Cantarell\", \"Fira Sans\", \"Droid Sans\", \"Helvetica Neue\", sans-serif",
        ),
        color(white),
        backgroundColor("202020" |> hex),
      ],
    );
    global("body, #root", [minHeight(100.0 |> vh)]);
    global("#root", [display(flexBox)]);
  };
};

Styles.globalStyles();

ReactDOMRe.renderToElementWithId(<App />, "root");

type parcelModule;
type hot;
[@bs.val] external parcelModule: parcelModule = "module";
[@bs.get] external hot: parcelModule => Js.nullable(hot) = "hot";
[@bs.send.pipe: hot] external accept: unit => unit = "accept";

switch (Js.Nullable.toOption(parcelModule |> hot)) {
| Some(h) => h |> accept()
| _ => ()
};
