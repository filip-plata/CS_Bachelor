package pl.edu.mimuw.forum.ui.bindings;

import javafx.beans.value.ObservableBooleanValue;

public class ToolbarBindings {

	private final Runnable onNew;
	private final Runnable onOpen;
	private final Runnable onSave;
	private final Runnable onUndo;
	private final Runnable onRedo;
	private final Runnable onAddNode;
	private final Runnable onDeleteNode;

	private final ObservableBooleanValue canCreateNew;
	private final ObservableBooleanValue canOpen;
	private final ObservableBooleanValue canSave;
	private final ObservableBooleanValue canUndo;
	private final ObservableBooleanValue canRedo;
	private final ObservableBooleanValue canAddNode;
	private final ObservableBooleanValue canDeleteNode;

	public ToolbarBindings(Runnable onNew, Runnable onOpen, Runnable onSave, Runnable onUndo, Runnable onRedo, Runnable onAddNode,
			Runnable onDeleteNode, ObservableBooleanValue canCreateNew, ObservableBooleanValue canOpen, ObservableBooleanValue canSave,
			ObservableBooleanValue canUndo, ObservableBooleanValue canRedo, ObservableBooleanValue canAddNode,
			ObservableBooleanValue canDeleteNode) {
		this.onNew = onNew;
		this.onOpen = onOpen;
		this.onSave = onSave;
		this.onUndo = onUndo;
		this.onRedo = onRedo;
		this.onAddNode = onAddNode;
		this.onDeleteNode = onDeleteNode;

		this.canCreateNew = canCreateNew;
		this.canOpen = canOpen;
		this.canSave = canSave;
		this.canUndo = canUndo;
		this.canRedo = canRedo;
		this.canAddNode = canAddNode;
		this.canDeleteNode = canDeleteNode;
	}

	public Runnable getOnNew() {
		return onNew;
	}

	public Runnable getOnOpen() {
		return onOpen;
	}

	public Runnable getOnSave() {
		return onSave;
	}

	public Runnable getOnUndo() {
		return onUndo;
	}

	public Runnable getOnRedo() {
		return onRedo;
	}

	public Runnable getOnAddNode() {
		return onAddNode;
	}

	public Runnable getOnDeleteNode() {
		return onDeleteNode;
	}

	public ObservableBooleanValue getCanCreateNew() {
		return canCreateNew;
	}
	
	public ObservableBooleanValue getCanOpen() {
		return canOpen;
	}

	public ObservableBooleanValue getCanSave() {
		return canSave;
	}

	public ObservableBooleanValue getCanUndo() {
		return canUndo;
	}

	public ObservableBooleanValue getCanRedo() {
		return canRedo;
	}

	public ObservableBooleanValue getCanAddNode() {
		return canAddNode;
	}

	public ObservableBooleanValue getCanDeleteNode() {
		return canDeleteNode;
	}

}
