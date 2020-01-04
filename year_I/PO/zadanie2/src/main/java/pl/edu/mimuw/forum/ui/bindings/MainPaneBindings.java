package pl.edu.mimuw.forum.ui.bindings;

import java.io.File;

import javafx.beans.binding.Bindings;
import javafx.beans.property.BooleanProperty;
import javafx.beans.property.ObjectProperty;
import javafx.beans.property.SimpleBooleanProperty;
import javafx.beans.property.SimpleObjectProperty;
import javafx.beans.value.ObservableBooleanValue;
import javafx.beans.value.ObservableObjectValue;
import javafx.beans.value.ObservableStringValue;

public class MainPaneBindings {

	private final ObjectProperty<File> file;
	private final BooleanProperty hasChanges;
	private final BooleanProperty undoAvailable;
	private final BooleanProperty redoAvailable;
	private final BooleanProperty nodeAdditionAvailable;
	private final BooleanProperty nodeRemovalAvailable;

	public MainPaneBindings() {
		this.file = new SimpleObjectProperty<>();
		this.hasChanges = new SimpleBooleanProperty();
		this.undoAvailable = new SimpleBooleanProperty();
		this.redoAvailable = new SimpleBooleanProperty();
		this.nodeAdditionAvailable = new SimpleBooleanProperty();
		this.nodeRemovalAvailable = new SimpleBooleanProperty();
	}

	public ObservableBooleanValue hasChanges() {
		return hasChanges;
	}

	public ObservableBooleanValue undoAvailable() {
		return undoAvailable;
	}

	public ObservableBooleanValue redoAvailable() {
		return redoAvailable;
	}

	public ObservableBooleanValue nodeAdditionAvailable() {
		return nodeAdditionAvailable;
	}
	
	public ObservableBooleanValue nodeRemovalAvailable() {
		return nodeRemovalAvailable;
	}

	public ObservableStringValue fileName() {
		return Bindings.createStringBinding(() -> file.get() == null ? "Untitled" : file.get().getName(), file);
	}

	public ObservableObjectValue<File> file() {
		return file;
	}

	public ObjectProperty<File> fileProperty() {
		return file;
	}

	public BooleanProperty hasChangesProperty() {
		return hasChanges;
	}

	public BooleanProperty undoAvailableProperty() {
		return undoAvailable;
	}

	public BooleanProperty redoAvailableProperty() {
		return redoAvailable;
	}

	public BooleanProperty nodeAdditionAvailableProperty() {
		return nodeAdditionAvailable;
	}
	
	public BooleanProperty nodeRemovaleAvailableProperty() {
		return nodeRemovalAvailable;
	}

}
