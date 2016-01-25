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

        public int CustomerDeposit { get; private set; }

        public int Overall => _balance.Overall;

        public IEnumerable<Tuple<Coin, int>> Stacks => _balance.Stacks;

        public VendingMachineBalance(IBalance balance)
        {
            _balance = (balance == null) ? Balance.ZeroBalance : balance;
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
                throw new VendingMachineException("Номинал монеты больше депозита.");

            var taken = _balance.TakeCoin(coin);
            CustomerDeposit -= (int)coin;

            return taken;
        }

        public IBalance TakeMoney(int money)
        {
            throw new NotImplementedException();
        }
    }
}
