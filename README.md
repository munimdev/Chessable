# Chessable: Chess Data Management System

## Project Description

Chessable is an advanced chess data management system designed to store, manage, and analyze a large database of grandmaster-level chess games. This project leverages sophisticated data structures to efficiently handle vast amounts of chess-related information, including details about players, events, game outcomes, openings, and specific moves played in each game.

The core objective of Chessable is to utilize these data structures not only for efficient data storage but also to perform complex queries and analytics, providing insights into chess strategies and trends among grandmasters.

### Key Features

- **Game Analysis**: Analyze games based on openings, player performance, and specific board positions.
- **Player Statistics**: Track player performance over time, including win rates, most played openings, and head-to-head matchups against other players.
- **Event Insights**: Access detailed information about chess events, including location, date, and associated games.
- **Opening Repertoire**: Explore detailed opening variations and their popularity among top-level players.
- **Dynamic Queries**: Perform on-the-fly queries to retrieve data based on complex criteria such as ECO codes, event locations, or specific moves.

### Data Structures

Chessable employs the following data structures to optimize data handling and operations:

- **Arrays**: Used for storing ordered data where quick access is required.
- **Linked Lists**: Ideal for dynamic data where insertions and deletions are frequent.
- **Hash Tables**: Facilitate rapid data retrieval and efficient data indexing for player and game lookups.
- **AVL Trees**: Ensure balanced data storage, providing efficient order statistics and optimized search times.

## Installation and Setup

1. **Clone the Repository**:
   ```bash
   git clone https://github.com/munimdev/Chessable.git
   cd Chessable
   ```

2. **Compile the Source Code** (Ensure you have a C++ compiler installed):
   ```bash
   g++ -o Chessable main.cpp -std=c++11
   ```

3. **Run the Application**:
   ```bash
   ./Chessable
   ```

## Usage

Chessable is designed with a menu-driven interface that allows users to perform various operations:

- **Get Openings by ECO**: Retrieve all openings associated with a specific ECO code.
- **Player Statistics**: View detailed statistics for a selected player.
- **Event Information**: Get detailed information about chess events.
- **Most Played Moves**: Analyze the most played moves from a given board position.

Users can navigate through the menu by entering the number corresponding to each action and following the on-screen prompts to enter additional details.

## Example

Here's a snapshot of an example operation using Chessable:

![output](https://github.com/munimdev/Chessable/blob/main/img/getMove1.jpg)

## Contributing

Contributions to Chessable are welcome! If you have suggestions for improvements or bug fixes, please fork the repository and submit a pull request.

1. **Fork the Repository**:
   Navigate to the GitHub page and click the "Fork" button.

2. **Create a Feature Branch**:
   ```bash
   git checkout -b new-feature
   ```

3. **Commit Your Changes**:
   ```bash
   git commit -am 'Add some feature'
   ```

4. **Push to the Branch**:
   ```bash
   git push origin new-feature
   ```

5. **Submit a Pull Request**:

## License

Chessable is released under the MIT License. See the LICENSE file for more details.

## Support

For support, contact [email](mailto:munimzafar100@gmail.com).

---
