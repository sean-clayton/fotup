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

[@react.component]
let make = (~uploads, ~removeFile) => {
  switch (uploads) {
  | [] => ReasonReact.null
  | _ =>
    <ul className=Styles.listContainer>
      {uploads
       |> List.map((upload: Api.upload) =>
            <UploadedFile removeFile upload key={upload.name} />
          )
       |> Array.of_list
       |> ReasonReact.array}
    </ul>
  };
};
