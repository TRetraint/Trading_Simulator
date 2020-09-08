# Trading_Simulator

## Introduction

Trading Simulator is a program for simulation of strategies and trading in condition. Developed in C++ and under the Qt framework, it gives access to several interesting features that I will detail below.

## Different features :

### 1. Displaying the data :

I had to display the data contained in a CSV file provided (containing the
OHCL values and Volume for one day of a company on the stock exchange, such as Facebook or Tesla). I displayed this data in the form of a graph in
candlestick for price data and in the form of a bar chart for volume information.

### 2. Choice of parameters :

oncerning the second function we had to allow the user to enter
simulation parameters such as the date on which the simulation will start, the date of the the amount it holds of the base currency and the counterpart currency (both in the base currency and the counterpart currency)
initialized by default). We also had to manage the intermediary's compensation.
with a percentage taken from each transaction.

### 3. Trading Strategy :

Then it was a question of being interested in the various Trading strategies. In order to do this I have implemented a table containing different information such as the date
of the transaction, the amount of the counterpart currency, the amount of the base currency and the amount of the transaction's than the return on investment.

### 4. Manual and automatic mode :
I then created a manual mode and an automatic mode. In the first one, it is the user who launches the program and can choose the dates of purchases and sales for a loaded pair as well as the cancellation of the last transaction. While in the automatic mode, the user selects a trading strategy. among a list of strategies studied. This last one will be applied automatically from the selected date to the end date. A decision (sale or purchase) is made according to OHLCV data.

### 5. Text Editor :
I also had a text editor to implement in order to allow the user to take notes on the strategies they have tested. With a backup option that can be consists of the data file, the table containing purchases and sales and the data file itself.

### 6. Technical indicators :
Concerning the technical indicators, I had to look at the various existing indicators in order to implement the automatic mode. Indeed, depending on the OHCLV data, purchase or sales decisions must be made.

### 7. Shape analysis :
For the shape analysis, the different types of candles had to be taken into account (mostly explained in the topic) and to code them.

### 8. Step by step mode :
Finally, I have made a step by step mode to make the simulation more realistic in manual. For a selected day, you can display the opening price as well as the decision to buy or sell without knowing future changes.

### 9. Evolution of the application :
In this part, I had to make sure that the application was capable of adapting itself to the data entered by the user without it crashing and facilitate the addition of components such as modifying the data format or adding new components such as automatic trading strategies and indicators.

## Architecture of the application
The application is structured around three main classes :
- The pilot class which contains the menu, allowing to choose :
. The file containing the data to load
. The date from which the data in the file will be displayed.
. The initial amounts that the user will have at his disposal
. Simulation mode: automatic or manual
But also to display the two other windows of the application: the graphics and the transaction manager.
Driver also contains the QWidget corresponding to the main window (the one with the graphics), to include them in it it has a pointer to an object of the EvolutionViewer class which can access all the graphs. This class has a method named LoadFile which will load all the data of a file whose path is given as a parameter. In addition, it will set to day the graphs in accordance with the date given to it in parameter: 10 candles, volumes and indicators will be displayed: the last one and the one corresponding to the chosen date, and the first 9 to the 9 courses preceding the course of the chosen date. 

- The EvolutionViewer class, containing all the charts (indicators, candle, volume) but also the interface allowing the user to access the information, from navigate (forward or backward) and take notes. This class has between other the attributes allowing the setting up of the candle chart: for theother graphics it has pointers to the objects responsible for them (to be detailed later): IndicatorsViewer, VolumeViewer and TransactionsViewer. Its main function is to update all graphs via its Update method. following the current day (given by its attribute last). With this method, the class manages the step by step mode, thanks to two buttons : nextDay and previousDay which call among others the Update method. The class allows to display the OHCL information of the candles with 4 QLineEdit: when you click on a candle, the corresponding information is written on it. The candles of the candlestick chart are managed by the Candle class, inheriting theQCandleStickSet class and also has a pointer on CoursOHCL allowing to to access the course information but also attributes containing the values of the different indicators, allowing us to display their exact values when you press down on a candle. In addition, the class has a multitude of method to determine which candle shape corresponds to the object, shape that is also displayed when you hold down a candle. It determines then if the candle is a Doji or a Hammer

- The TransactionsViewer class which is a transaction manager, it has pointers to a User object (which allows him to know how much money he has at his disposal), an EvolutionCours object to enable it to recover the rate of changes the current day given to it by a pointer to the last attribute of the object EvolutionViewer. The class displays: the current currency amounts owned by the user but also the return on investment. It also offers a QLineEdit to write the currency amount of desired counterparty to sell or buy by clicking on the buy or sell buttons. At each transaction, it displays in a new QLineEdit a sentence describing it. It manages this QLineEdit list thanks to an iterator allowing it to add and remove them easily, but also to browse through them. It has an updateTransactions method that removes the list of transactions. to redo a new one in accordance with the changes that may have occurred in the past. be performed, this method is used in the step-by-step mode to return to back.

Transactions are managed by several classes:

. The Transaction class, representing a transaction by two amounts, one in currency The other in the counterpart currency, which may be positive or negative, and a date. It has a method to translate its information into a QString, useful to display transactions in TransactionsViewer.
. Since the EvolutionTransactions class represents a list of transactions, it has an iterator on Transaction to be able to manage this list.
. The User class, representing the user of the program, has 4 amounts : two correspond to the amounts he currently has, the other two to the amounts he currently has. available at the beginning of the simulation (useful to calculate the return on investment) but it has above all an EvolutionTransaction object representing the list of the transactions he has made. To carry them out we use two methods buy and sell that allow to add the corresponding transaction in the EvolutionTransactions object all by checking if the requested transaction is possible. These methods have been used since TransactionsViewer class to update the QLineEdit list at the same time. Finally, the User class has an update method, called when you want to go back to back, it allows, by giving it a date as an argument, to restore all the transactions. made after this date, updating the amounts available to the user.

The Indicators are managed by several classes :

. The Indicator class represents the value of an indicator at a specific date (an object Indicator will thus correspond to the value of the rsi for a candle for example)
. The EvolutionIndicators class manages a series of Indicators, one Indicator for each course in the file. It uses an iterator on Indicator to manage this series.
. The RSI, EMA and MCAD classes which all inherit from EvolutionIndicateurs and which have each one a different calculation method allowing them, when given an object EvolutionCours, to calculate the indicators corresponding to each course of the object EvolutionCourses and put them in the list of Indicators.
. The IndicatorViewer class which displays a curve corresponding to the values of the indicators of each spark plug displayed in EvolutionViewer. This class allows to display the curve of any type of indicators, it is enough to have a method that updates the graph thanks to an EvolutionIndicator object and the current day's candle index displayed in EvolutionViewer (thanks to the last attribute). Thus thanks to the UpdateRSI method, UpdateEMA and UpdateMACD, the IndicatorViewer class can display a graph of any of these indicators.
. The VolumeViewer class manages the display of the bar graph representing the volume thanks to a series of QBarSeries.This series contains a set of QBarSet each representing one data. Like all Viewer classes, it has an Update method to display the volumes corresponding to the candles displayed in the EvolutionViewer class.
. The Note class is used to manage a note sheet. This note sheet is a ".txt" file that can be loaded or created if it does not exist thanks to the method "display_sheet_not()". This method opens a text editor containing the loaded file. You can then use the "save_note()" method to save the notes in a files. When saving the file will either be created if it did not exist, or the old one will be created. deleted and a new one will be created containing the notes taken. The signal "sauv_note()" is emitted after the backup, it will be captured by EvolutionViewer to adjust the size of the window after closing the text editor.
. The Simulation class allows the saving of a simulation (data, table of transactions and note sheet). The 3 elements can be saved separately, one can choose the one you want to save (you can also choose to save all 3). list of transactions are saved in ".txt" format. The data are as for them saved in ".csv" format in the same way as the ".csv" files received with the statement. When saving a simulation a new window opens where you can choose which item to save. Clicking on the "done" button closes the window, so that you have saved all items or not. Saving notes is done in the same way as saving the note sheet, the button will be captured by the attribute "Note* note_display" which will take care of the backup. For the data and the transaction list, pressing the button is captured by EvolutionViewer, which then sends another signal to Simulation, the latter has the information that needs to be saved (the data or the list and the number of transactions)

Transactions can be done either manually or automatically.

1. In summary, in manual mode :
First of all, in the transaction manager you choose the currency amount of counterparty to be exchanged. Then you choose whether you want to buy or sell. You can make as many transactions as you want per day and the change of day takes place manually (1 candle at a time) using the two buttons next Day and previous Day on the main window. If you choose to go back, the transactions of the following days will be displayed. to the current day will be deleted and the user's currency amounts updated adequately.

2. In automatic mode :
In contrast to the manual mode in the automatic mode it is not possible to choose the amount exchanged, the latter is fixed at 1000 currencies. In this mode it is possible to choose the number of transactions to be made and one transaction will be made per candle from the current candle (the number of candles does not necessarily correspond to the number of days, it is not happens that one day does not have a candle). Also each transaction is carried out based on the previous candle, because we assume that the day's data is not yet known. Thus the transaction of 03/04/19 will be carried out based on the candle of 02/04/19 if it exists, or on the closest one otherwise.

Finally, one can choose which strategy to use among the 6 proposed strategies:

- Variation_1Candle() : We look if the last candle represents an exchange rate increasing or decreasing. You buy if the rate is decreasing.
- Variation_3Candle(): We look if the last 3 candles represent a rate of change increasing or decreasing. You buy if the rate is decreasing on all the candles, and we sell if it is decreasing on all the candles. Otherwise we do not carry out of transaction.
- Variation_5Candle(): works the same way as the previous function but on the last 5 candles.
- Trend_5candle(): We look at the trend over the last 5 candles. That is to say that we look for each candle if it is increasing or decreasing. We choose then to buy if there are more taper candles and vice versa for sale. In any case there will be a transaction.
- Trend_10candle(): works in the same way as the previous function but on the last 10 candles.
- Comparison_mtn_10candle(): We compare the closing price of the last candle with the opening course of the 10th candle going up since today. then we buy if it is decreasing and sell if not.

## Conclusion

The realization of this project allowed me to face for the first time the creation from A to Z of a computer application of this scale. In addition to having developed new skills in object design and Human Machine Interface development, it allowed me to immerse myself in the trading environment. I am very satisfied with this project which has been very rewarding.
