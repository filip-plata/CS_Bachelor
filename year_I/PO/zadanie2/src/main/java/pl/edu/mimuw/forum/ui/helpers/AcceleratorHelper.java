package pl.edu.mimuw.forum.ui.helpers;

import com.sun.javafx.PlatformUtil;

import javafx.collections.ObservableMap;
import javafx.scene.Scene;
import javafx.scene.input.KeyCode;
import javafx.scene.input.KeyCodeCombination;
import javafx.scene.input.KeyCombination;
import pl.edu.mimuw.forum.ui.bindings.ToolbarBindings;

public class AcceleratorHelper {
	public static void SetUpAccelerators(Scene scene, ToolbarBindings bindings) {
		ObservableMap<KeyCombination, Runnable> accelerators = scene.getAccelerators();
		accelerators.put(new KeyCodeCombination(KeyCode.N, KeyCombination.SHORTCUT_DOWN), bindings.getOnNew());
		accelerators.put(new KeyCodeCombination(KeyCode.O, KeyCombination.SHORTCUT_DOWN), bindings.getOnOpen());
		accelerators.put(new KeyCodeCombination(KeyCode.S, KeyCombination.SHORTCUT_DOWN), bindings.getOnSave());
		accelerators.put(new KeyCodeCombination(KeyCode.Z, KeyCombination.SHORTCUT_DOWN), bindings.getOnUndo());
		if (PlatformUtil.isLinux() || PlatformUtil.isMac()) {
			accelerators.put(new KeyCodeCombination(KeyCode.Z, KeyCombination.SHORTCUT_DOWN, KeyCombination.SHIFT_DOWN),
					bindings.getOnRedo());
		} else { // Windows
			accelerators.put(new KeyCodeCombination(KeyCode.Y, KeyCombination.SHORTCUT_DOWN), bindings.getOnRedo());
		}
		accelerators.put(new KeyCodeCombination(KeyCode.K, KeyCombination.SHORTCUT_DOWN), bindings.getOnAddNode());
		accelerators.put(new KeyCodeCombination(KeyCode.L, KeyCombination.SHORTCUT_DOWN), bindings.getOnDeleteNode());
	}
}
