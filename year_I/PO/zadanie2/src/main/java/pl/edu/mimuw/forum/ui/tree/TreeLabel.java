package pl.edu.mimuw.forum.ui.tree;

import java.io.IOException;

import javafx.beans.binding.Bindings;
import javafx.fxml.FXML;
import javafx.fxml.FXMLLoader;
import javafx.scene.Node;
import javafx.scene.control.ContentDisplay;
import javafx.scene.control.Label;
import javafx.scene.control.TreeCell;
import pl.edu.mimuw.forum.exceptions.ApplicationException;
import pl.edu.mimuw.forum.ui.models.NodeViewModel;

public class TreeLabel extends TreeCell<NodeViewModel> {
	@FXML
	private Node pane;

	@FXML
	private Label type;

	@FXML
	private Label author;

	public TreeLabel() throws ApplicationException {
		FXMLLoader loader = new FXMLLoader(getClass().getResource("/fxml/tree_label.fxml"));
		loader.setRoot(this);
		loader.setController(this);

		try {
			loader.load();
		} catch (IOException e) {
			throw new ApplicationException(e);
		}

		setContentDisplay(ContentDisplay.GRAPHIC_ONLY);
	}

	@Override
	protected void updateItem(NodeViewModel item, boolean empty) {
		super.updateItem(item, empty);
		if (empty) {
			setGraphic(null);
		} else {
			setGraphic(pane);
			type.textProperty().bind(Bindings.createStringBinding(() -> {
				String text = item.getContent().get();
				return text.substring(0, Math.min(text.length(), 100));
			}, item.getContent()));
			author.textProperty().bind(item.getAuthor());
		}
	}

}
