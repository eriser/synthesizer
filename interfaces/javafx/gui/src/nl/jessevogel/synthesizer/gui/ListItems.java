package nl.jessevogel.synthesizer.gui;

import javafx.event.ActionEvent;
import javafx.event.EventHandler;
import javafx.scene.Node;
import javafx.scene.control.*;
import javafx.scene.input.ClipboardContent;
import javafx.scene.input.Dragboard;
import javafx.scene.input.TransferMode;
import javafx.scene.layout.Pane;

public class ListItems {

    public static Pane createLabel(String description) {
        Pane item = FXMLFiles.load("list_items/label.fxml");

        Label label = (Label) item.getChildren().get(0);
        label.setText(description);

        return item;
    }

    public static Pane createLabelValue(String description, String value) {
        Pane item = FXMLFiles.load("list_items/labelvalue.fxml");

        Label labelDescription = (Label) item.getChildren().get(0);
        labelDescription.setText(description);

        Label labelValue = (Label) item.getChildren().get(2);
        labelValue.setText(value);

        return item;
    }

    public static Pane createTextField(String description, String value, EventHandler<ActionEvent> handler) {
        Pane item = FXMLFiles.load("list_items/textfield.fxml");

        Label label = (Label) item.getChildren().get(0);
        label.setText(description);

        TextField textField = (TextField) item.getChildren().get(1);
        textField.setText(value);
        textField.setOnAction(handler);

        return item;
    }

    public static Pane createDraggable(String description, String value) {
        Pane item = FXMLFiles.load("list_items/draggable.fxml");

        Label label = (Label) item.getChildren().get(0);
        label.setText(description);

        item.setOnDragDetected(event -> {
            Dragboard db = ((Node) event.getSource()).startDragAndDrop(TransferMode.ANY);
            ClipboardContent content = new ClipboardContent();
            content.putString(value);
            db.setContent(content);
//            db.setDragView(new Image("/img/components/node.png"));
            event.consume();
        });

        return item;
    }

    public static Pane createHeader(String title) {
        Pane item = FXMLFiles.load("list_items/header.fxml");

        Label label = (Label) item.getChildren().get(0);
        label.setText(title);

        return item;
    }

    public static Pane createCheckBox(String description, boolean checked, EventHandler<ActionEvent> handler) {
        Pane item = FXMLFiles.load("list_items/checkbox.fxml");

        Label label = (Label) item.getChildren().get(0);
        label.setText(description);

        CheckBox checkBox = (CheckBox) item.getChildren().get(1);
        checkBox.setSelected(checked);
        checkBox.setOnAction(handler);

        return item;
    }

    public static Pane createChoiceBox(String description, String[] options, String value, EventHandler<ActionEvent> handler) {
        Pane item = FXMLFiles.load("list_items/choicebox.fxml");

        Label label = (Label) item.getChildren().get(0);
        label.setText(description);

        ChoiceBox choiceBox = (ChoiceBox) item.getChildren().get(1);
        for(String option : options)
            choiceBox.getItems().add(option);
        choiceBox.setValue(value);
        choiceBox.setOnAction(handler);

        return item;
    }

    public static Pane createButton(String text, EventHandler<ActionEvent> handler) {
        Pane item = FXMLFiles.load("list_items/button.fxml");

        Button button = (Button) item.getChildren().get(0);
        button.setText(text);
        button.setOnAction(handler);

        return item;
    }

    public static Pane createHr() {
        Pane item = FXMLFiles.load("list_items/hr.fxml");
        return item;
    }
}
