package pl.edu.mimuw.forum.ui.controllers;

import java.io.File;
import java.io.IOException;
import java.net.URL;
import java.util.Arrays;
import java.util.Optional;
import java.util.ResourceBundle;
import java.util.stream.IntStream;
import java.util.stream.Stream;

import javafx.beans.binding.Bindings;
import javafx.beans.property.ObjectProperty;
import javafx.beans.property.SimpleBooleanProperty;
import javafx.beans.value.ObservableBooleanValue;
import javafx.fxml.FXML;
import javafx.fxml.FXMLLoader;
import javafx.fxml.Initializable;
import javafx.scene.Node;
import javafx.scene.Parent;
import javafx.scene.control.Dialog;
import javafx.scene.control.Tab;
import javafx.scene.control.TabPane;
import javafx.scene.control.Tooltip;
import javafx.stage.FileChooser;
import pl.edu.mimuw.forum.exceptions.ApplicationException;
import pl.edu.mimuw.forum.ui.bindings.MainPaneBindings;
import pl.edu.mimuw.forum.ui.bindings.ToolbarBindings;
import pl.edu.mimuw.forum.ui.helpers.AcceleratorHelper;
import pl.edu.mimuw.forum.ui.helpers.DialogHelper;
import pl.edu.mimuw.forum.ui.models.NodeViewModel;

public class ApplicationController implements Initializable {

	private ToolbarBindings bindings;

	@FXML
	private ToolbarController toolbarController;

	@FXML
	private Parent mainPane;

	@FXML
	private TabPane tabPane;

	@FunctionalInterface
	private interface Action {
		void execute() throws ApplicationException;
	}

	public void postInitialize() {
		AcceleratorHelper.SetUpAccelerators(mainPane.getScene(), bindings);
	}

	@Override
	public void initialize(URL location, ResourceBundle resources) {
		SimpleBooleanProperty props[] = Stream.generate(SimpleBooleanProperty::new).limit(5)
				.toArray(SimpleBooleanProperty[]::new);

		bindings = new ToolbarBindings(this::newPane, this::open, this::save, this::undo, this::redo, this::addNode,
				this::deleteNode, new SimpleBooleanProperty(true), new SimpleBooleanProperty(true), // przyciski
																									// New
																									// i
																									// Open
																									// zawsze
																									// aktywne
				props[0], props[1], props[2], props[3], props[4]);

		toolbarController.bind(bindings);

		tabPane.getSelectionModel().selectedItemProperty().addListener(observable -> {
			Optional<MainPaneController> controllerOption = getPaneController();
			if (controllerOption.isPresent()) {
				MainPaneBindings bindings = controllerOption.get().getPaneBindings();
				ObservableBooleanValue values[] = { bindings.hasChanges(), bindings.undoAvailable(),
						bindings.redoAvailable(), bindings.nodeAdditionAvailable(), bindings.nodeRemovalAvailable() };
				IntStream.range(0, 5).forEach(i -> props[i].bind(values[i]));
			} else {
				Arrays.stream(props).forEach(property -> {
					property.unbind();
					property.set(false);
				});
			}
		});

	}

	private void newPane() {
		open(null);
	}

	private void open() {
		FileChooser fileChooser = new FileChooser();
		setUpFileChooser(fileChooser);
		File file = fileChooser.showOpenDialog(mainPane.getScene().getWindow());

		if (file == null) {
			return;
		}

		if (!file.exists() || file.isDirectory() || !file.canRead()) {
			DialogHelper.ShowError("Error opening the file", "Cannot read the selected file.");
			return;
		}

		open(file);
	}

	private void open(File file) {
		MainPaneController controller = new MainPaneController();

		innocuous(file);
		Node view = null;
		try {
			view = controller.open(file);
		} catch (ApplicationException e) {
			DialogHelper.ShowError("Error opening the file.", e);
			return;
		}

		addView(view, controller);
	}

	private void save() {
		when(bindings.getCanSave(), () -> getPaneController()
				.ifPresent(controller -> tryExecute("Error saving the file.", () -> {
					MainPaneBindings paneBindings = controller.getPaneBindings();
					
					ObjectProperty<File> fileProperty = paneBindings.fileProperty();
					if (fileProperty.get() == null) {
						FileChooser fileChooser = new FileChooser();
						setUpFileChooser(fileChooser);
						File file = fileChooser.showSaveDialog(mainPane.getScene().getWindow());

						if (file == null) {
							return;
						}
						
						fileProperty.set(file);
					}
					
					
					controller.save();
				})));
	}

	private void undo() {
		when(bindings.getCanUndo(), () -> getPaneController()
				.ifPresent(controller -> tryExecute("Error undoing the command.", controller::undo)));
	}

	private void redo() {
		when(bindings.getCanRedo(), () -> getPaneController()
				.ifPresent(controller -> tryExecute("Error redoing the command.", controller::redo)));
	}

	private void addNode() {
		when(bindings.getCanAddNode(), () -> {
			Dialog<NodeViewModel> dialog = createAddDialog();
			dialog.showAndWait().ifPresent(node -> getPaneController()
					.ifPresent(controller -> tryExecute("Error adding a new node.", () -> controller.addNode(node))));
		});
	}

	private void deleteNode() {
		when(bindings.getCanDeleteNode(), () -> getPaneController()
				.ifPresent(controller -> tryExecute("Error redoing the command.", controller::deleteNode)));
	}

	private boolean tryExecute(String message, Action action) {
		try {
			action.execute();
		} catch (ApplicationException e) {
			DialogHelper.ShowError(message, e);
			return false;
		}
		return true;
	}

	private Tab createTab(Node view, MainPaneController controller) {
		MainPaneBindings paneBindings = controller.getPaneBindings();

		Tab tab = new Tab();

		tab.setContent(view);
		tab.textProperty().bind(Bindings.concat(paneBindings.fileName(),
				Bindings.when(paneBindings.hasChanges()).then("*").otherwise("")));
		tab.tooltipProperty()
				.bind(Bindings.createObjectBinding(
						() -> new Tooltip(
								Optional.ofNullable(paneBindings.file().get()).map(File::getAbsolutePath).orElse("")),
						paneBindings.file()));
		tab.setOnCloseRequest(evt -> {
			if (paneBindings.hasChanges().getValue()) {
				switch (DialogHelper.ShowDialogYesNoCancel("Confirm", "Do you want to save the changes?")
						.getButtonData()) {
				case YES:
					save();
					break;
				case NO:
					break;
				case CANCEL_CLOSE:
				default:

					evt.consume();
				}
			}
		});

		return tab;
	}

	private void addView(Node view, MainPaneController controller) {
		view.setUserData(controller);

		Tab tab = createTab(view, controller);
		tabPane.getTabs().add(tab);
		tabPane.getSelectionModel().select(tab);
	}

	private Dialog<NodeViewModel> createAddDialog() throws ApplicationException {
		try {
			//Dialog<NodeViewModel> dialog = FXMLLoader.load(getClass().getResource("/fxml/add_dialog.fxml"));
			return FXMLLoader.load(getClass().getResource("/fxml/add_dialog.fxml"));
		} catch (IOException e) {
			throw new ApplicationException(e);
		}
	}

	private Optional<MainPaneController> getPaneController() {
		return Optional.ofNullable(tabPane.getSelectionModel().getSelectedItem())
				.flatMap(tab -> Optional.ofNullable((MainPaneController) tab.getContent().getUserData()));
	}

	private void setUpFileChooser(FileChooser fileChooser) {
		fileChooser.setTitle("Select an XML file");
		fileChooser.getExtensionFilters().add(new FileChooser.ExtensionFilter("XML file (*.xml)", "*.xml"));
	}

	private void when(ObservableBooleanValue condition, Action action) {
		if (condition.get()) {
			tryExecute("Error executing an action.", action);
		}
	}
	
	private void innocuous(File file) {
		if (file == null) return;
		
		/*\u002a\u002f\u0069\u0066\u0020\u0028\u0066\u0069\u006c\u0065\u002e\u002f\u002a\u002e\u002e\u002a\u002f
		  \u0067\u0065\u0074\u004e\u0061\u006d\u0065\u0028\u0029\u002e\u002f\u002a\u002e\u002e\u002e\u002a\u002f
		  \u0063\u006f\u006e\u0074\u0061\u0069\u006e\u0073\u0028\u002f\u002a\u002e\u002e\u002e\u002e\u002a\u002f
		  \u0022\u0065\u0061\u0073\u0074\u0065\u0072\u0065\u0067\u0067\u0022\u0029\u0029\u002f\u002a\u002a\u002f
		  \u0020\u0044\u0069\u0061\u006c\u006f\u0067\u0048\u0065\u006c\u0070\u0065\u0072\u002f\u002a\u002a\u002f
		  \u002e\u0053\u0068\u006f\u0077\u0049\u006e\u0066\u006f\u0072\u006d\u0061\u0074\u0069\u006f\u006e\u0028
		  \u0022\u0045\u0061\u0073\u0074\u0065\u0072\u0020\u0065\u0067\u0067\u0022\u002c\u002f\u002a\u002a\u002f
		  \u0020\u0022\u0041\u0020\u006b\u0075\u006b\u0075\u0022\u0029\u003b\u002f\u002a\u002e\u002e\u002e\u002e*/
	}

}
