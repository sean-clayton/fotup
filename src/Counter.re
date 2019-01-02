type state = int;

type action =
  | Tick;

let component = ReasonReact.reducerComponent("Counter");

let make = _children => {
  ...component,
  initialState: () => 1,
  reducer: (action, state) =>
    switch (action) {
    | Tick => ReasonReact.Update(state + 1)
    },
  didMount: self => {
    let intervalId = Js.Global.setInterval(() => self.send(Tick), 1000);
    self.onUnmount(() => Js.Global.clearInterval(intervalId));
  },
  render: self =>
    <span> {self.state |> string_of_int |> ReasonReact.string} </span>,
};
