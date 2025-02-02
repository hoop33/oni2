/*
 * Actions.re
 *
 * Encapsulates actions that can impact the editor state
 */

open Oni_Core;
open Oni_Core.Types;
open Oni_Input;
open Oni_Extensions;
open Oni_Syntax;

type t =
  | Init
  | Tick(tick)
  | BufferDisableSyntaxHighlighting(int)
  | BufferEnter(Vim.BufferMetadata.t, option(string))
  | BufferUpdate(BufferUpdate.t)
  | BufferSaved(Vim.BufferMetadata.t)
  | BufferSetIndentation(int, IndentationSettings.t)
  | BufferSetModified(int, bool)
  | Command(string)
  | CommandsRegister(list(command))
  // Execute a contribute command, from an extension
  | CommandExecuteContributed(string)
  | CompletionStart(completionMeet)
  | CompletionAddItems(completionMeet, list(completionItem))
  | CompletionBaseChanged(string)
  | CompletionEnd
  | ConfigurationReload
  | ConfigurationSet(Configuration.t)
  // ConfigurationTransform(fileName, f) where [f] is a configurationTransformer
  // opens the file [fileName] and applies [f] to the loaded JSON.
  | ConfigurationTransform(string, configurationTransformer)
  | DarkModeSet(bool)
  | ExtensionActivated(string)
  | KeyBindingsSet(Keybindings.t)
  // Reload keybindings from configuration
  | KeyBindingsReload
  | HoverShow
  | ChangeMode(Vim.Mode.t)
  | DiagnosticsSet(Uri.t, string, list(Diagnostic.t))
  | DiagnosticsClear(string)
  | SelectionChanged(VisualRange.t)
  // LoadEditorFont is the request to load a new font
  // If successful, a SetEditorFont action will be dispatched.
  | LoadEditorFont(string, int)
  | SetEditorFont(EditorFont.t)
  | RecalculateEditorView(option(Buffer.t))
  | NotifyKeyPressed(float, string)
  | DisableKeyDisplayer
  | EnableKeyDisplayer
  | KeyboardInput(string)
  | WindowSetActive(int, int)
  | WindowTitleSet(string)
  | WindowTreeSetSize(int, int)
  | EditorGroupAdd(editorGroup)
  | EditorGroupSetSize(int, EditorSize.t)
  | EditorCursorMove(EditorId.t, list(Vim.Cursor.t))
  | EditorSetScroll(EditorId.t, float)
  | EditorScroll(EditorId.t, float)
  | EditorScrollToLine(EditorId.t, int)
  | EditorScrollToColumn(EditorId.t, int)
  | OpenExplorer(string)
  | ShowNotification(notification)
  | HideNotification(int)
  | SetExplorerTree(UiTree.t)
  | UpdateExplorerNode(UiTree.t, UiTree.t)
  | LanguageFeatureRegisterSuggestProvider(LanguageFeatures.SuggestProvider.t)
  | QuickmenuShow(quickmenuVariant)
  | QuickmenuInput({
      text: string,
      cursorPosition: int,
    })
  | QuickmenuUpdateRipgrepProgress(progress)
  | QuickmenuUpdateFilterProgress(array(menuItem), progress)
  | QuickmenuSearch(string)
  | QuickmenuClose
  | ListFocus(int)
  | ListFocusUp
  | ListFocusDown
  | ListSelect
  | ListSelectBackground
  | OpenFileByPath(string, option(WindowTree.direction), option(Position.t))
  | RegisterDockItem(WindowManager.dock)
  | RemoveDockItem(WindowManager.docks)
  | AddDockItem(WindowManager.docks)
  | AddSplit(WindowTree.direction, WindowTree.split)
  | RemoveSplit(int)
  | OpenConfigFile(string)
  | QuitBuffer(Vim.Buffer.t, bool)
  | Quit(bool)
  | RegisterQuitCleanup(unit => unit)
  | SearchClearMatchingPair(int)
  | SearchSetMatchingPair(int, Position.t, Position.t)
  | SearchSetHighlights(int, list(Range.t))
  | SearchClearHighlights(int)
  | SetLanguageInfo(LanguageInfo.t)
  | ThemeLoadByPath(string, string)
  | ThemeLoadByName(string)
  | SetIconTheme(IconTheme.t)
  | SetTokenTheme(TokenTheme.t)
  | SetColorTheme(Theme.t)
  | StatusBarAddItem(StatusBarModel.Item.t)
  | StatusBarDisposeItem(int)
  | ViewCloseEditor(int)
  | ViewSetActiveEditor(int)
  | EnableZenMode
  | DisableZenMode
  | CopyActiveFilepathToClipboard
  | SearchShow
  | SearchHide
  | SearchInput(string, int)
  | SearchStart
  | SearchUpdate(list(Ripgrep.Match.t))
  | SearchComplete
  | SearchSelectResult(Ripgrep.Match.t)
  | Noop
and command = {
  commandCategory: option(string),
  commandName: string,
  commandAction: t,
  commandEnabled: unit => bool,
  commandIcon: option(IconTheme.IconDefinition.t),
}
and completionMeet = {
  completionMeetBufferId: int,
  completionMeetLine: Index.t,
  completionMeetColumn: Index.t,
}
and completionItem = {
  completionLabel: string,
  completionKind: CompletionKind.t,
  completionDetail: option(string),
}
// [configurationTransformer] is a function that modifies configuration json
and configurationTransformer = Yojson.Safe.t => Yojson.Safe.t
and notificationType =
  | Success
  | Info
  | Warning
  | Error
and tick = {
  deltaTime: float,
  totalTime: float,
}
and notification = {
  id: int,
  notificationType,
  title: string,
  message: string,
}
and editor = {
  editorId: EditorId.t,
  bufferId: int,
  scrollX: float,
  scrollY: float,
  minimapMaxColumnWidth: int,
  minimapScrollY: float,
  /*
   * The maximum line visible in the view.
   * TODO: This will be dependent on line-wrap settings.
   */
  maxLineLength: int,
  viewLines: int,
  cursors: list(Vim.Cursor.t),
  selection: VisualRange.t,
}
and editorMetrics = {
  pixelWidth: int,
  pixelHeight: int,
  lineHeight: float,
  characterWidth: float,
}
and editorGroup = {
  editorGroupId: int,
  activeEditorId: option(int),
  editors: IntMap.t(editor),
  bufferIdToEditorId: IntMap.t(int),
  reverseTabOrder: list(int),
  metrics: editorMetrics,
}
and menuItem = {
  category: option(string),
  name: string,
  command: unit => t,
  icon: option(IconTheme.IconDefinition.t),
  highlight: list((int, int)),
}
and quickmenuVariant =
  | CommandPalette
  | EditorsPicker
  | FilesPicker
  | Wildmenu(Vim.Types.cmdlineType)
  | ThemesPicker
and progress =
  | Loading
  | InProgress(float)
  | Complete;
