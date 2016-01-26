using System;
using System.Collections.Generic;
using VendingMachine.Finance;
using VendingMachine.Products;
using VendingMachine.Exceptions;

namespace VendingMachine.Roles
{
    class VendingMachineSeller : ISeller
    {
        private Menu _vendingMachineMenu;
        private VendingMachineBalance _vendingMachineBalance;

        public int CustomerDeposit =>
            _vendingMachineBalance.CustomerDeposit;

        public IEnumerable<Product> VarietyOfProducts =>
            _vendingMachineMenu.AvaliableProducts;

        public VendingMachineSeller(Menu menu, VendingMachineBalance balance)
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

        public void Configure(VendingMachineBalance balance)
        {
            if (balance == null) throw new NullReferenceException();

            _vendingMachineBalance = balance;
        }

        #endregion

        #region Custumer Interaction

        public void AcceptPayment(Coin coin)
        {
            _vendingMachineBalance.PutCoin(coin);
        }

        public Product DispenceProduct(Guid barcode)
        {
            int price = _vendingMachineMenu.GetPrice(barcode);

            if (price > CustomerDeposit)
                throw new NotEnoughMoneyVendingMachineException();

            _vendingMachineBalance.CustomerDeposit -= price;

            return _vendingMachineMenu.TakeOne(barcode);
        }

        public IBalance ReturnDeposit()
        {
            return _vendingMachineBalance.TakeMoney(_vendingMachineBalance.CustomerDeposit);
        }

        #endregion
    }
}
