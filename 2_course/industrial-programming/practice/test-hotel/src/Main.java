import java.io.File;
import java.io.FileNotFoundException;
import java.util.*;
import java.util.stream.Collectors;

public class Main {
    private static final String INPUT_FILE = "hotel.txt";

    static class HotelCollection {
        private Set<Hotel> hotels = new HashSet<>();

        public HotelCollection(Set<Hotel> initialHotels) {
            if (initialHotels != null) {
                this.hotels.addAll(initialHotels);
            }
        }

        public void add(Hotel hotel) {
            hotels.add(hotel);
        }

        public void printHotels() {
            Set<String> cities = hotels.stream()
                    .map(h -> h.city)
                    .collect(Collectors.toSet());

            List<String> sortedCities = new ArrayList<>(cities);
            sortedCities.sort(Comparator.naturalOrder());

            System.out.println("--- Список отелей по городам (Города по алфавиту, Отели по убыванию звезд) ---");

            for (String city : sortedCities) {
                List<Hotel> localHotels = hotels.stream()
                        .filter(hotel -> hotel.city.equals(city))
                        .collect(Collectors.toList());

                localHotels.sort(
                        (Hotel a, Hotel b) -> b.starQuantity.compareTo(a.starQuantity)
                );

                System.out.println("Город: " + city);
                for (Hotel hotel : localHotels) {
                    System.out.println("  " + hotel.name + " - " + hotel.starQuantity + "★");
                }
            }
            System.out.println("---------------------------------------------------------------------------------");
        }

        public void findByCity(String city) {
            System.out.println("--- Поиск отелей в городе: " + city + " (По убыванию звезд) ---");

            List<Hotel> localHotels = hotels.stream()
                    .filter(hotel -> hotel.city.equalsIgnoreCase(city))
                    .collect(Collectors.toList());

            if (localHotels.isEmpty()) {
                System.out.println("Отели в городе '" + city + "' не найдены.");
                System.out.println("-------------------------------------------------------");
                return;
            }

            localHotels.sort(
                    (Hotel a, Hotel b) -> b.starQuantity.compareTo(a.starQuantity)
            );

            for (Hotel hotel : localHotels) {
                System.out.println("  " + hotel.name + " - " + hotel.starQuantity + "★");
            }

            System.out.println("-------------------------------------------------------");
        }

        public void findByHotel(String name) {
            System.out.println("--- Поиск городов, где есть отель: " + name + " ---");

            List<Hotel> matchingHotels = hotels.stream()
                    .filter(hotel -> hotel.name.equalsIgnoreCase(name))
                    .collect(Collectors.toList());

            if (matchingHotels.isEmpty()) {
                System.out.println("Отель с названием '" + name + "' не найден ни в одном городе.");
                System.out.println("---------------------------------------------------------------");
                return;
            }

            Set<String> cities = matchingHotels.stream()
                    .map(h -> h.city)
                    .collect(Collectors.toSet());

            System.out.println("Отель '" + name + "' найден в городах:");
            for (String city : cities) {
                System.out.println("  - " + city);
            }

            System.out.println("---------------------------------------------------------------");
        }
    }

    static class Hotel {
        public String city;
        public String name;
        public Integer starQuantity;

        public Hotel(String city, String name, Integer starQuantity) {
            this.city = city;
            this.name = name;
            this.starQuantity = starQuantity;
        }

        @Override
        public String toString() {
            return city + " " + name + " " + starQuantity + "★";
        }

        @Override
        public boolean equals(Object o) {
            if (this == o) return true;
            if (o == null || getClass() != o.getClass()) return false;
            Hotel hotel = (Hotel) o;
            return starQuantity.equals(hotel.starQuantity) &&
                    Objects.equals(city, hotel.city) &&
                    Objects.equals(name, hotel.name);
        }

        @Override
        public int hashCode() {
            return Objects.hash(city, name, starQuantity);
        }
    }

    static class FileUtils {
        public static Set<Hotel> readFile(String filename) {
            Set<Hotel> hotels = new HashSet<>();
            try {
                File file = new File(filename);
                Scanner sc = new Scanner(file);

                while (sc.hasNextLine()) {
                    String line = sc.nextLine();
                    if (line.trim().isEmpty()) continue;

                    try {
                        String[] parts = line.split(";");
                        if (parts.length == 3) {
                            String city = parts[0].trim();
                            String name = parts[1].trim();
                            Integer starQuantity = Integer.parseInt(parts[2].trim());

                            hotels.add(new Hotel(city, name, starQuantity));
                        } else {
                            System.err.println("Ошибка: Неверный формат строки (ожидается 3 части): " + line);
                        }
                    } catch (NumberFormatException e) {
                        System.err.println("Ошибка: Количество звезд не является числом: " + line);
                    }
                }
                sc.close();
            } catch (FileNotFoundException e) {
                System.err.println("Файл не найден по пути: " + filename);
            }
            return hotels;
        }
    }

    public static void main(String[] args) {
        System.out.println("Запуск программы по обработке данных отелей...");

        Set<Hotel> initialHotels = FileUtils.readFile(INPUT_FILE);

        initialHotels.add(new Hotel("Берлин", "Hilton", 4));
        initialHotels.add(new Hotel("Париж", "Hilton", 4));


        if (initialHotels.isEmpty()) {
            System.err.println("Невозможно продолжить. Данные об отелях не загружены.");
        }

        HotelCollection collection = new HotelCollection(initialHotels);

        collection.printHotels();

        collection.findByCity("Санкт-Петербург");
        collection.findByCity("Москва");
        collection.findByCity("Нью-Йорк");

        collection.findByHotel("Hilton");
        collection.findByHotel("Заря");
        collection.findByHotel("Несуществующий");
    }
}