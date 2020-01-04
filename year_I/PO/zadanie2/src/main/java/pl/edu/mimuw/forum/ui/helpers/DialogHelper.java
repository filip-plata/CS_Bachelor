package pl.edu.mimuw.forum.ui.helpers;

import java.io.PrintWriter;
import java.io.StringWriter;

import javafx.scene.control.Alert;
import javafx.scene.control.Alert.AlertType;
import javafx.scene.control.ButtonType;
import javafx.scene.control.Label;
import javafx.scene.control.TextArea;
import javafx.scene.layout.GridPane;
import javafx.scene.layout.Priority;

public class DialogHelper {
	public static void ShowError(String header, String context, Throwable throwable) {
		Alert alert = new Alert(AlertType.ERROR);

		alert.setTitle("Error");
		alert.setHeaderText(header);
		
		if (context == null) {
			context = throwable == null ? "" : throwable.getMessage();
		}
		
		alert.setContentText(context);
		
		if (throwable != null) {
			StringWriter stringWriter = new StringWriter();
			PrintWriter printWriter = new PrintWriter(stringWriter);
			throwable.printStackTrace(printWriter);
			String exceptionText = stringWriter.toString();

			Label label = new Label("The exception stacktrace was:");

			TextArea textArea = new TextArea(exceptionText);
			textArea.setEditable(false);
			textArea.setWrapText(true);

			textArea.setMaxWidth(Double.MAX_VALUE);
			textArea.setMaxHeight(Double.MAX_VALUE);
			GridPane.setVgrow(textArea, Priority.ALWAYS);
			GridPane.setHgrow(textArea, Priority.ALWAYS);

			GridPane expContent = new GridPane();
			expContent.setMaxWidth(Double.MAX_VALUE);
			expContent.add(label, 0, 0);
			expContent.add(textArea, 0, 1);
			
			alert.getDialogPane().setExpandableContent(expContent);
		}

		alert.showAndWait();
	}
	
	public static void ShowError(String header, Throwable throwable) {
		ShowError(header, null, throwable);
	}
	
	public static void ShowError(String header, String context) {
		ShowError(header, context, null);
	}
	
	public static ButtonType ShowDialogYesNoCancel(String title, String message) {
		Alert dialog = new Alert(AlertType.CONFIRMATION);
		dialog.setTitle(title);
		dialog.setHeaderText(message);

		dialog.getButtonTypes().setAll(ButtonType.YES, ButtonType.NO, ButtonType.CANCEL);
		
		return dialog.showAndWait().orElse(ButtonType.CANCEL);
	}
	
	public static void ShowInformation(String title, String message) {
		Alert dialog = new Alert(AlertType.INFORMATION);
		dialog.setTitle(title);
		dialog.setHeaderText(message);

		dialog.showAndWait();
	}
}
