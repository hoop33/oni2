open Actions;

type t = {
  variant,
  query: string,
  prefix: option(string),
  cursorPosition: int,
  items: array(menuItem),
  filterProgress: progress,
  ripgrepProgress: progress,
  focused: option(int) // TODO: Might not be a great idea to use an index to refer to a specific item in an array that changes over time
}

and variant =
  Actions.quickmenuVariant =
    | CommandPalette
    | EditorsPicker
    | FilesPicker
    | Wildmenu(Vim.Types.cmdlineType)
    | ThemesPicker;

let defaults = variant => {
  variant,
  query: "",
  prefix: None,
  cursorPosition: 0,
  focused: None,
  items: [||],
  filterProgress: Complete,
  ripgrepProgress: Complete,
};

// TODO: This doesn't really belong here. Find a better home for it.
let getLabel = (item: menuItem) => {
  switch (item.category) {
  | Some(v) => v ++ ": " ++ item.name
  | None => item.name
  };
};
