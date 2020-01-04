package pl.edu.mimuw.forum.ui.helpers;

import java.text.ParseException;
import java.text.SimpleDateFormat;
import java.time.LocalDate;
import java.time.LocalDateTime;
import java.time.LocalTime;
import java.time.ZoneId;
import java.util.Date;

import javafx.beans.property.ObjectProperty;
import javafx.beans.property.SimpleObjectProperty;
import javafx.scene.control.DatePicker;
import javafx.scene.control.TextField;
import javafx.scene.input.KeyCode;
import javafx.scene.input.KeyEvent;
import javafx.util.StringConverter;

public class DateTimePicker extends DatePicker {
	private SimpleDateFormat formatter = new SimpleDateFormat("yyyy-MM-dd HH:mm");

	private SimpleObjectProperty<Date> dateTimeValue = new SimpleObjectProperty<>(new Date());

	@SuppressWarnings("deprecation")
	public DateTimePicker() {
		setConverter(new StringConverter<LocalDate>() {

			@Override
			public String toString(LocalDate object) {
				return dateTimeValue.get() != null ? formatter.format(dateTimeValue.get()) : "";
			}

			@Override
			public LocalDate fromString(String string) {
				if (string == null) {
					dateTimeValue.set(null);
					return null;
				}

				try {
					dateTimeValue.set(formatter.parse(string));
				} catch (ParseException e) {
					dateTimeValue.set(null);
					return null;
				}

				return LocalDateTime.ofInstant(dateTimeValue.get().toInstant(), ZoneId.systemDefault()).toLocalDate();
			}

		});

		valueProperty().addListener((observable, oldValue, newValue) -> {
			if (newValue == null) {
				dateTimeValue.set(null);
			} else {
				Date d = ToDate(LocalDateTime.of(newValue, LocalTime.now()));
				Date oldDate = dateTimeValue.get();
				if (oldDate != null) {
					d.setHours(oldDate.getHours());
					d.setMinutes(oldDate.getMinutes());
				}
				dateTimeValue.set(d);
			}
		});

		dateTimeValue.addListener((observable, oldValue, newValue) -> {
			getEditor().setText(formatter.format(newValue));
		});

		getEditor().textProperty().addListener((observable, oldValue, newValue) -> {
			try {
				dateTimeValue.set(formatter.parse(newValue));
			} catch (ParseException e) {
				
			}
		});

		getEditor().focusedProperty().addListener((observable, oldValue, newValue) -> {
			if (newValue != null) {
				simulateEnterPressed();
			}
		});

	}

	private void simulateEnterPressed() {
		TextField editor = getEditor();
		editor.fireEvent(new KeyEvent(editor, editor, KeyEvent.KEY_PRESSED, null, null, KeyCode.ENTER, false, false,
				false, false));
	}

	public ObjectProperty<Date> dateTimeValueProperty() {
		return dateTimeValue;
	}

	private static Date ToDate(LocalDateTime dt) {
		return Date.from(dt.atZone(ZoneId.systemDefault()).toInstant());
	}

	/*
	 * private static LocalDate ToLocalDate(Date dt) { return
	 * LocalDateTime.ofInstant(dt.toInstant(),
	 * ZoneId.systemDefault()).toLocalDate(); }
	 */

}