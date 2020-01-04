package pl.edu.mimuw.forum.ui;

import java.io.IOException;

import javafx.application.Application;
import javafx.application.Platform;
import javafx.fxml.FXMLLoader;
import javafx.scene.Parent;
import javafx.scene.Scene;
import javafx.scene.image.Image;
import javafx.stage.Stage;
import pl.edu.mimuw.forum.ui.controllers.ApplicationController;
import pl.edu.mimuw.forum.ui.helpers.DialogHelper;
import pl.edu.mimuw.forum.ui.helpers.TextInputControlBindingWorkaround;

public class Main extends Application {
	@Override
	public void start(Stage primaryStage) {
		Thread.currentThread().setUncaughtExceptionHandler((thread, throwable) -> {
			Platform.runLater(() -> DialogHelper.ShowError("Unhandled error has occurred.", throwable));
		});
		
		setUserAgentStylesheet(STYLESHEET_MODENA);

		TextInputControlBindingWorkaround.DeactivateUndoRedoShortcuts();

		FXMLLoader loader = new FXMLLoader(getClass().getResource("/fxml/application.fxml"));

		Parent root = null;
		try {
			root = (Parent) loader.load();
		} catch (IOException e) {
			e.printStackTrace();
			System.exit(-1);
		}

		ApplicationController controller = (ApplicationController) loader.getController();

		Scene scene = new Scene(root);

		primaryStage.setTitle("XML Forum editor");
		primaryStage.setScene(scene);
		primaryStage.getIcons().add(new Image("/images/group.png"));

		controller.postInitialize();

		primaryStage.show();
	}

	public static void main(String[] args) {
		launch(args);
	}
}
