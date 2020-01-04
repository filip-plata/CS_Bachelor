package pl.edu.mimuw.forum.ui.helpers;

import java.util.Iterator;

import com.sun.javafx.scene.control.behavior.KeyBinding;
import com.sun.javafx.scene.control.behavior.TextInputControlBindings;

public class TextInputControlBindingWorkaround extends TextInputControlBindings {

	public static void DeactivateUndoRedoShortcuts() {
		Iterator<KeyBinding> iterator = BINDINGS.iterator();
		while (iterator.hasNext()) {
			String action = iterator.next().getAction();
			if (action.equals("Undo") || action.equals("Redo")) {
				iterator.remove();
			}
		}
	}
}
