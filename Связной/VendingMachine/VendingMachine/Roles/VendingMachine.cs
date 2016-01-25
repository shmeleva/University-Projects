using System;
using VendingMachine.Finance;
using VendingMachine.Products;

namespace VendingMachine.Roles
{
    class VendingMachine : ISeller
    {
        private Menu _vendingMachineMenu;
        private IBalance _vendingMachineBalance;

        public VendingMachine(Menu menu, IBalance balance)
        {
            Configure(menu);
            Configure(balance);
        }

        #region Configuration

        public void Configure(Menu menu)
        {
            if (menu == null) throw new NullReferenceException();

            _vendingMachineMenu = menu;
        }

        public void Configure(IBalance balance)
        {
            if (balance == null) throw new NullReferenceException();

            _vendingMachineBalance = balance;
        }

        #endregion

        #region Custumer Interaction

        public void PutMoney(Coin coin)
        {
            _vendingMachineBalance.PutCoin(coin);
        }

        public Product GetItem(string barcode)
        {
            throw new NotImplementedException();
        }

        public IBalance RequestChange()
        {
            throw new NotImplementedException();
        }

        #endregion
    }
}
