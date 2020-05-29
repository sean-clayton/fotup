module Styles = {
  open Css;

  let alert =
    style([
      backgroundColor(rgba(200, 24, 24, 0.5)),
      borderTop(3->px, solid, tomato),
      padding(1.0->rem),
      textShadow(Shadow.text(~y=1->px, ~blur=2->px, rgba(0, 0, 0, 1.0))),
      marginBottom(4.0->rem),
    ]);

  let alertContent =
    style([display(`flex), justifyContent(`center), alignItems(`center)]);
};

[@react.component]
let make = (~children=?) => {
  <div className=Styles.alert>
    <div className=Styles.alertContent>
      {children->Belt.Option.getWithDefault(React.string(""))}
    </div>
  </div>;
};
