using System;
using System.Collections.Generic;
using System.Linq;
using VendingMachine.Exceptions;

namespace VendingMachine.Finance
{
    class Balance : IBalance
    {
        private readonly Dictionary<Coin, int> _stacksOfCoins = new Dictionary<Coin, int>();

        public int Overall =>
            _stacksOfCoins.Sum(s => (int)s.Key * s.Value);

        public IDictionary<Coin, int> StacksOfCoins =>
            _stacksOfCoins.Where(s => s.Value != 0).ToDictionary(s => s.Key, s => s.Value);

        public Balance() { }

        public Balance(IEnumerable<Coin> coins)
        {
            if (coins == null)
                return;

            var aggregatedByDenomination = from coin in coins
                                           group coin by coin into grouping
                                           select new { Coin = grouping.Key, Amount = grouping.Count() };

            foreach (var item in aggregatedByDenomination)
                _stacksOfCoins.Add(item.Coin, item.Amount);
        }

        public Balance(IDictionary<Coin, int> coins)
        {
            if (coins == null)
                return;

            foreach (var item in coins)
                _stacksOfCoins.Add(item.Key, item.Value);
        }

        public void PutCoin(Coin coin)
        {
            PutCoins(coin, 1);
        }

        public void PutCoins(Coin coin, int numberOfCoins)
        {
            if (numberOfCoins < 0)
                throw new ArgumentException("The number of coins is less than zero.");

            if (_stacksOfCoins.ContainsKey(coin) == false)
                _stacksOfCoins.Add(coin, 0);

            _stacksOfCoins[coin] += numberOfCoins;
        }

        public void PutMoney(IBalance money)
        {
            foreach (var stack in money.StacksOfCoins)
                PutCoins(stack.Key, stack.Value);
        }

        public Coin TakeCoin(Coin coin)
        {
            if (_stacksOfCoins.ContainsKey(coin) == false || _stacksOfCoins[coin] == 0)
                throw new NoCoinBalanceException();

            _stacksOfCoins[coin]--;

            return coin;
        }

        public IBalance TakeMoney(int amount)
        {
            Dictionary<Coin, int> balance = CombineCoins(amount);

            foreach (var stack in balance)
                for (int i = 0; i < stack.Value; i++)
                    TakeCoin(stack.Key);

            return new Balance(balance);
        }

        private Dictionary<Coin, int> CombineCoins(int amount)
        {
            var change = new Dictionary<Coin, int>();

            foreach (var stack in _stacksOfCoins.OrderByDescending(s => s.Key))
            {
                if (stack.Value > 0)
                {
                    int numberOfDenominations = (amount / (int)stack.Key);

                    if (numberOfDenominations == 0)
                    {
                        continue;
                    }

                    if (numberOfDenominations > stack.Value)
                    {
                        numberOfDenominations = stack.Value;
                        amount -= numberOfDenominations * (int)stack.Key;
                    }
                    else
                    {
                        amount = (amount % (int)stack.Key);
                    }

                    change.Add(stack.Key, numberOfDenominations);
                }
            }

            if (amount > 0)
            {
                throw new NoCoinBalanceException();
            }

            return change;
        }
    }
}
