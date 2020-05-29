type variant = [ | `danger | `warn];

module Styles = {
  open Css;

  let variants = variant =>
    switch (variant) {
    | `danger =>
      style([
        backgroundColor(rgba(200, 24, 24, 0.5)),
        borderTop(3->px, solid, tomato),
      ])
    | `warn =>
      style([
        backgroundColor(rgba(200, 128, 24, 0.5)),
        borderTop(3->px, solid, goldenrod),
      ])
    };

  let alert = variant =>
    merge([
      variants(variant),
      style([
        padding(1.0->rem),
        textShadow(Shadow.text(~y=1->px, ~blur=2->px, rgba(0, 0, 0, 1.0))),
        marginBottom(2.0->rem),
      ]),
    ]);

  let alertContent =
    style([display(`flex), justifyContent(`center), alignItems(`center)]);
};

[@react.component]
let make = (~children=?, ~variant: variant=`danger) => {
  <div className={Styles.alert(variant)}>
    <div className=Styles.alertContent>
      {children->Belt.Option.getWithDefault(React.string(""))}
    </div>
  </div>;
};
