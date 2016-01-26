using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using VendingMachine.Exceptions;

namespace VendingMachine.Finance
{
    class VendingMachineBalance : IBalance
    {
        private readonly IBalance _balance;

        private int _customerDeposit;
        public int CustomerDeposit
        {
            get
            {
                return _customerDeposit;
            }
            set
            {
                if (value < 0)
                    throw new NotEnoughMoneyBalanceException();

                _customerDeposit = value;
            }
        }

        public int Overall => _balance.Overall;

        public IDictionary<Coin, int> StacksOfCoins => _balance.StacksOfCoins;

        public VendingMachineBalance(IBalance balance)
        {
            _balance = (balance == null) ? new Balance() : balance;
        }

        public VendingMachineBalance(IDictionary<Coin, int> coins)
        {
            _balance = new Balance(coins);
        }

        public VendingMachineBalance(IEnumerable<Coin> coins)
        {
            _balance = new Balance(coins);
        }

        public void PutCoin(Coin coin)
        {
            _balance.PutCoin(coin);
            CustomerDeposit += (int)coin;
        }

        public void PutCoins(Coin coin, int numberOfCoins)
        {
            _balance.PutCoins(coin, numberOfCoins);
            CustomerDeposit += (int)coin * numberOfCoins;
        }

        public void PutMoney(IBalance money)
        {
            _balance.PutMoney(money);
            CustomerDeposit += money.Overall;
        }

        public Coin TakeCoin(Coin coin)
        {
            if (CustomerDeposit < (int)coin)
                throw new NotEnoughMoneyBalanceException();

            var taken = _balance.TakeCoin(coin);
            CustomerDeposit -= (int)coin;

            return taken;
        }

        public IBalance TakeMoney(int amount)
        {
            if (CustomerDeposit < amount)
            {
                throw new NotEnoughMoneyBalanceException();
            }

            try
            {
                IBalance balance = _balance.TakeMoney(amount);
                CustomerDeposit -= amount;

                return balance;
            }
            catch(NoCoinBalanceException)
            {
                throw new NoChangeVendingMachineException();
            }
        }
    }
}
