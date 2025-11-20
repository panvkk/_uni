package org.example;

import javafx.application.Platform;
import javafx.scene.layout.VBox;
import javafx.scene.paint.Color;
import javafx.scene.shape.Rectangle;

import java.util.Arrays;

public class SortingData {
    private final Integer[] array;
    private final VBox container;
    private final double containerWidth;
    private final Color color;
    private final int maxValue;

    public SortingData(Integer[] initialArray, VBox container, double containerWidth, Color color) {
        this.array = Arrays.copyOf(initialArray, initialArray.length);
        this.container = container;
        this.containerWidth = containerWidth;
        this.color = color;
        this.maxValue = Arrays.stream(initialArray).max(Integer::compare).orElse(1);
    }

    public Integer[] getArray() {
        return array;
    }

    /**
     * Перерисовывает полоски на основе текущего состояния массива.
     * @param index1 Индекс первого элемента для подсветки (или null).
     * @param index2 Индекс второго элемента для подсветки (или null).
     */
    public void updateVisualization(Integer index1, Integer index2) {
        if (container == null) return;

        Platform.runLater(() -> {
            container.getChildren().clear();

            // Расчет фактической или минимальной высоты полоски
            double calculatedHeight = container.getHeight() / array.length;
            // Устанавливаем минимальную высоту в 1.0 пиксель
            double rectHeight = Math.max(1.0, calculatedHeight);

            for (int i = 0; i < array.length; i++) {
                // Ширина полоски (пропорционально значению)
                double rectWidth = (double) array[i] / maxValue * containerWidth;

                // Минимальная ширина, чтобы полоска была видна
                rectWidth = Math.max(1.0, rectWidth);

                Rectangle rect = new Rectangle(rectWidth, rectHeight);
                rect.setFill(color);

                // Подсветка элементов
                if (index1 != null && index2 != null && (i == index1 || i == index2)) {
                    rect.setFill(Color.web("#FF0000")); // Ярко-красный
                }

                // Добавляем полоску
                container.getChildren().add(rect);
            }
        });
    }
}