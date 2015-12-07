using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace DesignPatterns
{
    /// <summary>
    /// Создание армии.
    /// Фасад для UnitFactory<LightUnit> и UnitFactory<HeavyUnit> и синглтон.
    /// </summary>
    class ArmyFactoryFacade
    {
        private static readonly UnitFactory<LightUnit> _lightUnitFactory = UnitFactory<LightUnit>.Instance; // @ static
        private static readonly UnitFactory<HeavyUnit> _heavyUnitFactory = UnitFactory<HeavyUnit>.Instance;

        public delegate Unit CreateUnit(Int32 cost);

        private class UnitTypeInfo
        {
            public CreateUnit Create
            {
                get;
                private set;
            }

            public Int32 Probability
            {
                get;
                private set;
            }

            public UnitTypeInfo(CreateUnit create, Int32 probability)
            {
                Create = create;
                Probability = probability;
            }
        }

        private readonly UnitTypeInfo[] _creates = new UnitTypeInfo[] // @
        {
            new UnitTypeInfo(_lightUnitFactory.CreateUnit, 2),
            new UnitTypeInfo(_heavyUnitFactory.CreateUnit, 1)
        };

        public Int32 LowestCost
        {
            get
            {
                return Math.Min(_lightUnitFactory.LowestCost, _heavyUnitFactory.LowestCost); //@
            }
        }

        private ArmyFactoryFacade() { }

        private static ArmyFactoryFacade _instance;
        public static ArmyFactoryFacade Instance
        {
            get
            {
                if (_instance == null)
                {
                    _instance = new ArmyFactoryFacade();
                }
                return _instance;
            }
        }

        /// <summary>
        /// Вовзращает индекс с списке типов следующего для создания юнита.
        /// </summary>
        /// <param name="cost">Индекс типа юнита. Отрицательное значение, если создание невозможно.</param>
        /// <returns></returns>
        private Int32 NextTypeIndex(Int32 cost)
        {
            if (LowestCost > cost)
                return -1;

            Int32 sum = 0;
            foreach (var elem in _creates)
                sum += elem.Probability;

            Int32 random = BattleRandom.Next(sum);
            Int32 accumulation = 0;

            for (Int32 i = 0; i < _creates.Length; i++)
            {
                accumulation += _creates[i].Probability;
                if (random < accumulation)
                    return i;
            }

            return _creates.Length - 1;
        }

        public List<Unit> CreateArmy(Int32 cost)
        {
            List<Unit> army = new List<Unit>();
            Int32 index;

            while (cost >= LowestCost)
            {
                index = NextTypeIndex(cost);

                if (index < 0)
                    break;

                Unit unit = _creates[index].Create(cost);

                if (unit != null)
                {
                    cost -= unit.Cost;
                    army.Add(unit);
                }
            }

            return army;
        }
    }
}
