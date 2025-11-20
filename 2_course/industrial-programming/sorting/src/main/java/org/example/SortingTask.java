package org.example;

import javafx.application.Platform;
import javafx.scene.control.Label;

import java.text.SimpleDateFormat;
import java.util.Date;

public class SortingTask implements Runnable {
    private final SortingData data;
    private final String sortType;
    private final Label statusLabel;
    private final Runnable completionCallback;
    // Увеличиваем задержку для лучшей визуализации
    private static final int VISUALIZATION_DELAY_MS = 20;

    public SortingTask(SortingData data, String sortType, Label statusLabel, Runnable completionCallback) {
        this.data = data;
        this.sortType = sortType;
        this.statusLabel = statusLabel;
        this.completionCallback = completionCallback;
    }

    /**
     * Форматирует время в миллисекундах в читаемый формат HH:mm:ss.SSS.
     */
    private String formatTime(long timestampMs) {
        SimpleDateFormat sdf = new SimpleDateFormat("HH:mm:ss.SSS");
        return sdf.format(new Date(timestampMs));
    }

    private void sleepForVisualization() {
        try {
            Thread.sleep(VISUALIZATION_DELAY_MS);
        } catch (InterruptedException e) {
            Thread.currentThread().interrupt();
        }
    }

    @Override
    public void run() {
        long startTime = System.currentTimeMillis();
        // Используем форматированное время
        Platform.runLater(() -> statusLabel.setText(
                String.format("⏰ %s: Начало в %s", sortType, formatTime(startTime))
        ));

        Integer[] arr = data.getArray();
        switch (sortType) {
            case "Пузырьком":
                bubbleSort(arr);
                break;
            case "Вставками":
                insertionSort(arr);
                break;
            case "Быстрая":
                quickSort(arr, 0, arr.length - 1);
                break;
        }

        long endTime = System.currentTimeMillis();
        long duration = endTime - startTime;

        data.updateVisualization(null, null);

        // Используем форматированное время
        Platform.runLater(() -> {
            statusLabel.setText(String.format(
                    "✅ %s: Завершено в %s\nДлительность: %s мс",
                    sortType, formatTime(endTime), duration
            ));
            completionCallback.run();
        });
    }

    // --- Методы Сортировки (добавлены вызовы sleepForVisualization) ---

    private void bubbleSort(Integer[] arr) {
        int n = arr.length;
        for (int i = 0; i < n - 1; i++) {
            for (int j = 0; j < n - i - 1; j++) {
                if (arr[j] > arr[j + 1]) {
                    int temp = arr[j];
                    arr[j] = arr[j + 1];
                    arr[j + 1] = temp;
                }
                data.updateVisualization(j, j + 1);
                sleepForVisualization();
            }
        }
    }

    private void insertionSort(Integer[] arr) {
        int n = arr.length;
        for (int i = 1; i < n; ++i) {
            int key = arr[i];
            int j = i - 1;
            while (j >= 0 && arr[j] > key) {
                arr[j + 1] = arr[j];
                j = j - 1;
                data.updateVisualization(j + 1, j + 2);
                sleepForVisualization();
            }
            arr[j + 1] = key;
            data.updateVisualization(j + 1, i);
            sleepForVisualization();
        }
    }

    private void quickSort(Integer[] arr, int low, int high) {
        if (low < high) {
            int pi = partition(arr, low, high);
            quickSort(arr, low, pi - 1);
            quickSort(arr, pi + 1, high);
        }
    }

    private int partition(Integer[] arr, int low, int high) {
        int pivot = arr[high];
        int i = (low - 1);
        for (int j = low; j < high; j++) {
            if (arr[j] <= pivot) {
                i++;
                int temp = arr[i];
                arr[i] = arr[j];
                arr[j] = temp;
                data.updateVisualization(i, j);
                sleepForVisualization();
            } else {
                data.updateVisualization(high, j);
                sleepForVisualization();
            }
        }
        int temp = arr[i + 1];
        arr[i + 1] = arr[high];
        arr[high] = temp;
        data.updateVisualization(i + 1, high);
        sleepForVisualization();
        return i + 1;
    }
}