module Styles = {
  open Css;

  let listContainer =
    style([
      display(flexBox),
      flexDirection(column),
      padding(1.0->rem),
      margin(zero),
      marginTop(1.0->rem),
      listStyleType(none),
      backgroundColor(rgba(0, 0, 0, 0.25)),
    ]);
};

let component = ReasonReact.statelessComponent("Uploads");

let make = (~uploads, _children) => {
  ...component,
  render: _self =>
    switch (uploads) {
    | [] => ReasonReact.null
    | _ =>
      <ul className=Styles.listContainer>
        {uploads
         |> List.map((upload: Api.upload) =>
              <UploadedFile upload key={upload.name} />
            )
         |> Array.of_list
         |> ReasonReact.array}
      </ul>
    },
};
