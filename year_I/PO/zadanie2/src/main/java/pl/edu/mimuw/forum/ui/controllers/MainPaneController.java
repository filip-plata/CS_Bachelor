package pl.edu.mimuw.forum.ui.controllers;

import java.io.File;
import java.io.FileNotFoundException;
import java.io.IOException;
import java.io.PrintWriter;
import java.io.UnsupportedEncodingException;
import java.net.URL;
import java.util.List;
import java.util.Optional;
import java.util.ResourceBundle;

import com.thoughtworks.xstream.XStream;

import javafx.beans.binding.Bindings;
import javafx.beans.binding.BooleanBinding;
import javafx.fxml.FXML;
import javafx.fxml.FXMLLoader;
import javafx.fxml.Initializable;
import javafx.scene.Node;
import javafx.scene.control.TreeItem;
import javafx.scene.control.TreeView;
import pl.edu.mimuw.forum.data.Forum;
import pl.edu.mimuw.forum.exceptions.ApplicationException;
import pl.edu.mimuw.forum.ui.bindings.MainPaneBindings;
import pl.edu.mimuw.forum.ui.helpers.DialogHelper;
import pl.edu.mimuw.forum.ui.models.NodeViewModel;
import pl.edu.mimuw.forum.ui.tree.ForumTreeItem;
import pl.edu.mimuw.forum.ui.tree.TreeLabel;
import pl.edu.mimuw.forum.undoRedoManager.ChangeDataAdd;
import pl.edu.mimuw.forum.undoRedoManager.ChangeDataRemove;
import pl.edu.mimuw.forum.undoRedoManager.UndoRedoManager;

public class MainPaneController implements Initializable {

	private NodeViewModel document;

	private MainPaneBindings bindings;
	
	private UndoRedoManager undoRedoManager;
	

	@FXML
	private TreeView<NodeViewModel> treePane;

	@FXML
	private DetailsPaneController detailsController;

	public MainPaneController() {
		bindings = new MainPaneBindings();
	}

	@Override
	public void initialize(URL location, ResourceBundle resources) {
		BooleanBinding nodeSelectedBinding = Bindings.isNotNull(treePane.getSelectionModel().selectedItemProperty());
		bindings.nodeAdditionAvailableProperty().bind(nodeSelectedBinding);
		bindings.nodeRemovaleAvailableProperty()
				.bind(nodeSelectedBinding.and(
						Bindings.createBooleanBinding(() -> getCurrentTreeItem().orElse(null) != treePane.getRoot(),
								treePane.rootProperty(), nodeSelectedBinding)));
		
		
		bindings.hasChangesProperty().set(true);
		bindings.undoAvailableProperty().set(false);	
		bindings.redoAvailableProperty().set(false);
		
		undoRedoManager = new UndoRedoManager(this);
		
		detailsController.passUndoRedoManager(undoRedoManager);

	}

	public MainPaneBindings getPaneBindings() {
		return bindings;
	}
	
	private XStream initializeXML() {
		XStream xstream = new XStream();
		xstream.addImplicitCollection(pl.edu.mimuw.forum.data.Node.class, "children", pl.edu.mimuw.forum.data.Node.class);
		xstream.alias("Forum", pl.edu.mimuw.forum.data.Forum.class);
		return xstream;
	}

	public Node open(File file) throws ApplicationException {
		
		if (file != null) {
			
			XStream xstream = initializeXML();
			
			pl.edu.mimuw.forum.data.Forum doc = (pl.edu.mimuw.forum.data.Forum) xstream.fromXML(file);
			document = doc.getModel();
		} else {
			document = new NodeViewModel("Welcome to a new forum", "Admin");
		}
		
		getPaneBindings().fileProperty().set(file);

		return openInView(document);
	}

	public void save() throws ApplicationException {
		if (document != null) {
			String file =  bindings.fileName().getValue();
			String extension = file.substring(file.lastIndexOf('.') + 1, file.length());
			
			if (!extension.equals("xml")) {
				System.err.println("Wrong file extension");
				throw new ApplicationException();
			}
			
			PrintWriter pw = null;
			
			try {
				pw = new PrintWriter(file, "UTF-8");
			} catch (FileNotFoundException e) {
				e.printStackTrace();
			} catch (UnsupportedEncodingException e) {
				e.printStackTrace();
			}
			XStream xstream = initializeXML();
			
			String xml = xstream.toXML(new Forum(document.toNode()));

			pw.print(xml);
			pw.close();
			bindings.hasChangesProperty().set(false);
			
			undoRedoManager.clearHistory();
		}
	}
	
	public void undo() throws ApplicationException {
		undoRedoManager.undoLastChange();
	}

	public void redo() throws ApplicationException {
		undoRedoManager.redoLastChange();
	}

	public void addNode(NodeViewModel node) throws ApplicationException {
		bindings.hasChangesProperty().set(true);
		getCurrentNode().ifPresent(currentlySelected -> {
			currentlySelected.getChildren().add(node);		// Zmieniamy jedynie model, widok (TreeView) jest aktualizowany z poziomu
															// funkcji nasluchujacej na zmiany w modelu (zob. metode createViewNode ponizej)
		});
	}
	
	public void addNode(ForumTreeItem parent, NodeViewModel node) throws ApplicationException {
		
		parent.getValue().getChildren().add(node);
	}

	public void deleteNode() {
		bindings.hasChangesProperty().set(true);
		getCurrentTreeItem().ifPresent(currentlySelectedItem -> {
			TreeItem<NodeViewModel> parent = currentlySelectedItem.getParent();

			NodeViewModel parentModel;
			NodeViewModel currentModel = currentlySelectedItem.getValue();
			if (parent == null) {
				return; // Blokujemy usuniecie korzenia - TreeView bez korzenia jest niewygodne w obsludze
			} else {
				parentModel = parent.getValue();
				parentModel.getChildren().remove(currentModel); // Zmieniamy jedynie model, widok (TreeView) jest aktualizowany z poziomu
																// funkcji nasluchujacej na zmiany w modelu (zob. metode createViewNode ponizej)
			}

		});
	}
	
	public void deleteNode(NodeViewModel parent, NodeViewModel child) {
		
		parent.getChildren().remove(child);
	}

	private Node openInView(NodeViewModel document) throws ApplicationException {
		Node view = loadFXML();

		treePane.setCellFactory(tv -> {
			try {
				return new TreeLabel();
			} catch (ApplicationException e) {
				DialogHelper.ShowError("Error creating a tree cell.", e);
				return null;
			}
		});


		ForumTreeItem root = createViewNode(document);
		root.addEventHandler(TreeItem.<NodeViewModel> childrenModificationEvent(), event -> {
		});

		treePane.setRoot(root);

		for (NodeViewModel w : document.getChildren()) {
			addToTree(w, root);
		}

		expandAll(root);

		treePane.getSelectionModel().selectedItemProperty()
				.addListener((observable, oldValue, newValue) -> onItemSelected(oldValue, newValue));

		return view;
	}
	
	private Node loadFXML() throws ApplicationException {
		FXMLLoader loader = new FXMLLoader();
		loader.setController(this);
		loader.setLocation(getClass().getResource("/fxml/main_pane.fxml"));

		try {
			return loader.load();
		} catch (IOException e) {
			throw new ApplicationException(e);
		}
	}
	
	private Optional<? extends NodeViewModel> getCurrentNode() {
		return getCurrentTreeItem().<NodeViewModel> map(TreeItem::getValue);
	}

	private Optional<TreeItem<NodeViewModel>> getCurrentTreeItem() {
		return Optional.ofNullable(treePane.getSelectionModel().getSelectedItem());
	}

	private void addToTree(NodeViewModel node, ForumTreeItem parentViewNode, int position) {
		ForumTreeItem viewNode = createViewNode(node);

		List<TreeItem<NodeViewModel>> siblings = parentViewNode.getChildren();
		siblings.add(position == -1 ? siblings.size() : position, viewNode);

		node.getChildren().forEach(child -> addToTree(child, viewNode));
	}

	private void addToTree(NodeViewModel node, ForumTreeItem parentViewNode) {
		addToTree(node, parentViewNode, -1);
	}

	private void removeFromTree(ForumTreeItem viewNode) {
		viewNode.removeChildListener();
		TreeItem<NodeViewModel> parent = viewNode.getParent();
		if (parent != null) {
			viewNode.getParent().getChildren().remove(viewNode);
		} else {
			treePane.setRoot(null);
		}
	}

	private ForumTreeItem createViewNode(NodeViewModel node) {
		ForumTreeItem viewNode = new ForumTreeItem(node);
		viewNode.setChildListener(change -> {	// wywolywanem, gdy w modelu dla tego wezla zmieni sie zawartosc kolekcji dzieci
			while (change.next()) {
				if (change.wasAdded()) {
					int i = change.getFrom();
					for (NodeViewModel child : change.getAddedSubList()) {
						undoRedoManager.memorize(new ChangeDataAdd(child, viewNode, i));
						addToTree(child, viewNode, i);	// uwzgledniamy nowy wezel modelu w widoku
						i++;
					}
				}

				if (change.wasRemoved()) {
					for (int i = change.getFrom(); i <= change.getTo(); ++i) {
						undoRedoManager.memorize(new ChangeDataRemove(viewNode.getChildren().get(i).getValue(), viewNode, i));
						removeFromTree((ForumTreeItem) viewNode.getChildren().get(i)); // usuwamy wezel modelu z widoku
					}
				}
			}
		});

		return viewNode;
	}

	private void expandAll(TreeItem<NodeViewModel> item) {
		item.setExpanded(true);
		item.getChildren().forEach(this::expandAll);
	}

	private void onItemSelected(TreeItem<NodeViewModel> oldItem, TreeItem<NodeViewModel> newItem) {
		undoRedoManager.setDummyAction();
		detailsController.setModel(newItem != null ? newItem.getValue() : null);
		undoRedoManager.unsetDummyAction();
	}
	
	public MainPaneBindings getBindings() {
		return bindings;
	}

}
