using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace DesignPatterns
{
    class BattleInterruptController
    {
		private readonly Battlefield _field;
		private readonly Int32 _treshold;

        private Int32 _counter;
        private Int32 _redsPreviousMovCount;
		private Int32 _whitesPreviousMoveCount;

		public Boolean UnitsAreBored
        {
            get;
            private set;
        }

        public BattleInterruptController(Battlefield field, Int32 treshold)
		{
            _field = field;
			_treshold = treshold;

            _redsPreviousMovCount = _field.Reds.Count;
            _whitesPreviousMoveCount = _field.Whites.Count;
		}

		public void Update()
		{
            _counter = (_redsPreviousMovCount == _field.Reds.Count && _whitesPreviousMoveCount == _field.Whites.Count) ? _counter + 1 : 0;

            _redsPreviousMovCount = _field.Reds.Count;
            _whitesPreviousMoveCount = _field.Whites.Count;

            UnitsAreBored = _counter > _treshold;
		}
    }
}
