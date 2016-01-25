using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace VendingMachine.Finance
{
    interface IBalance
    {
        int Overall { get; }

        IEnumerable<Tuple<Coin, int>> Stacks { get; }

        void PutCoin(Coin coin);
        void PutCoins(Coin coin, int numberOfCoins);
        void PutMoney(IBalance money);

        Coin TakeCoin(Coin coin);
        IBalance TakeMoney(int money);
    }
}
