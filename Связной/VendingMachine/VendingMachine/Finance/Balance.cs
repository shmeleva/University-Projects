using System;
using System.Collections.Generic;
using System.Linq;

namespace VendingMachine.Finance
{
    class Balance : IBalance
    {
        protected readonly Dictionary<Coin, int> _stacks = new Dictionary<Coin, int>();

        public int Overall => _stacks.Sum(coin => (int)coin.Key * coin.Value);

        public IEnumerable<Tuple<Coin, int>> Stacks
        {
            get
            {
                var filled = _stacks.Where(stack => stack.Value > 0);

                foreach (var stack in filled)
                    yield return new Tuple<Coin, int>(stack.Key, stack.Value);
            }
        }

        public static Balance ZeroBalance => new Balance();

        private Balance() { }

        public Balance(IEnumerable<Coin> coins)
        {
            if (coins == null) return;

            var aggregatedByDenomination = from coin in coins
                                           group coin by coin into grouping
                                           select new { Coin = grouping.Key, Number = grouping.Count() };

            foreach (var item in aggregatedByDenomination)
                _stacks.Add(item.Coin, item.Number);
        }

        public Balance(IDictionary<Coin, int> coins)
        {
            if (coins == null) return;

            foreach (var item in coins)
                _stacks.Add(item.Key, item.Value);
        }

        public void PutCoin(Coin coin)
        {
            PutCoins(coin, 1);
        }

        public void PutCoins(Coin coin, int numberOfCoins)
        {
            if (numberOfCoins < 0)
                throw new ArgumentException("The number of coins is less than zero.");

            if (_stacks.ContainsKey(coin) == false)
                _stacks.Add(coin, 0);

            _stacks[coin] += numberOfCoins;
        }

        public void PutMoney(IBalance money)
        {
            foreach (var stack in money.Stacks)
                PutCoins(stack.Item1, stack.Item2);
        }

        public Coin TakeCoin(Coin coin)
        {
            if (_stacks.ContainsKey(coin) == false || _stacks[coin] == 0)
                throw new Exception("Отсутствуют монеты с указанным номиналом.");

            _stacks[coin]--;

            return coin;
        }

        public IBalance TakeMoney(int money)
        {
            throw new NotImplementedException();
        }
    }
}
