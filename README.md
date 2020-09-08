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


