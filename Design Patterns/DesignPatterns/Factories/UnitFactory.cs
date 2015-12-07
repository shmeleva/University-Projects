using System;
using System.Collections.Generic;
using System.Linq;
using System.Reflection;
using System.Text;
using System.Threading.Tasks;

namespace DesignPatterns
{
    /// <summary>
    /// Фабрика для создания юнитов и синглтон.
    /// </summary>
    /// <typeparam name="T">LightUnit или HeavyUnit</typeparam>
    class UnitFactory<T> : IUnitFactory where T : Unit
    {
        private class UnitTypeInfo : IComparable
        {
            public ConstructorInfo Ctor { get; private set; }
            public Int32 Cost { get; private set; }
            public Int32 Probability { get; set; }

            public UnitTypeInfo(ConstructorInfo ctor, Int32 cost, Int32 probability = 0)
            {
                Ctor = ctor;
                Cost = cost;
                Probability = probability;
            }

            public int CompareTo(object obj)
            {
                if (obj == null) return 1;

                UnitTypeInfo otherTemperature = obj as UnitTypeInfo;
                if (otherTemperature != null)
                    return this.Cost.CompareTo(otherTemperature.Cost);
                else
                    throw new ArgumentException();
            }
        }

        private const Int32 _defaultHealth = 100;
        private readonly List<UnitTypeInfo> _units;

        private static UnitFactory<T> _instance;
        public static UnitFactory<T> Instance
        {
            get
            {
                if (_instance == null)
                {
                    _instance = new UnitFactory<T>();
                }
                return _instance;
            }
        }

        private UnitFactory()
        {
            Assembly a = Assembly.GetExecutingAssembly();
            _units = new List<UnitTypeInfo>();

            foreach (Type t in a.GetTypes())
            {
                if (t.BaseType == typeof(T))
                {
                    var ctor = t.GetConstructor(new[] { typeof(Int32) });

                    if (ctor != null)
                    {
                        Unit temp = ctor.Invoke(new object[] { _defaultHealth }) as Unit;

                        if (temp != null)
                        {
                            _units.Add(new UnitTypeInfo(ctor, temp.Cost));
                        }
                    }
                }
            }

            // Сортировка по стоимости юнита.
            _units.Sort();

            // Вычисление шансов создания каждого юнита.
            CalculateProbabilities();
        }

        /// <summary>
        /// Расчет вероятностей создания юнита для каждого типа.
        /// </summary>
        private void CalculateProbabilities()
        {
            if (_units.Count == 0)
                return;

            // Создание массива для вычисления вероятности.
            Int32[] probabilities = new Int32[_units.Count];
            for (int i = 0; i < _units.Count; i++)
                probabilities[i] = _units[i].Cost;

            // Нахождение обратно пропорциональных стоимости значений.
            Int32 lcm = ExtendedMath.AggregatedLcm(probabilities.ToArray());
            for (int i = 0; i < probabilities.Length; i++)
                probabilities[i] = lcm / probabilities[i];

            // Сохранение значений в _units.
            for (int i = 0; i < _units.Count; i++)
                _units[i].Probability = probabilities[i];
        }

        /// <summary>
        /// Вовзращает индекс с списке типов следующего для создания юнита.
        /// </summary>
        /// <param name="cost">Индекс типа юнита.</param>
        /// <returns></returns>
        private Int32 NextTypeIndex(Int32 cost)
        {
            Int32 sum = 0, number = 0;

            foreach (var elem in _units)
            {
                if (elem.Cost > cost)
                    break;

                sum += elem.Probability;
                number++;
            }

            Int32 random = BattleRandom.Next(sum), accumulation = 0;

            for (Int32 i = 0; i < number; i++)
            {
                accumulation += _units[i].Probability;

                if (random < accumulation)
                    return i;
            }

            return -1;
        }

        #region IUnitFactory

        public Int32 LowestCost
        {
            get
            {
                return _units.Count > 0 ? _units[0].Cost : Int32.MaxValue;
            }
        }

        public Unit CreateUnit(Int32 cost)
        {
            Int32 index = NextTypeIndex(cost);
            return index < 0 ? null : _units[index].Ctor.Invoke(new object[] { _defaultHealth }) as Unit;
        }

        #endregion
    }
}
