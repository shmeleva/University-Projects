using System.Collections.Generic;

namespace VendingMachine.Finance
{
    interface IBalance
    {
        int Overall { get; }
        IDictionary<Coin, int> StacksOfCoins { get; }

        void PutCoin(Coin coin);
        void PutCoins(Coin coin, int numberOfCoins);
        void PutMoney(IBalance money);

        Coin TakeCoin(Coin coin);
        IBalance TakeMoney(int amount);
    }
}
