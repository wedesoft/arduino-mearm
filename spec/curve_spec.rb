require_relative '../curve'

describe Linear do
  let :curve do
    Linear.new 0
  end

  it 'should start with the given value' do
    expect(curve.get).to be 0
  end

  it 'should stay constant over time' do
    expect(curve.advance(5).get).to be 0
  end

  context 'when moving forward' do
    let :curve do
      Linear.new(100, speed: 10).target 200
    end

    it 'should advance with time' do
      expect(curve.advance(5).get).to be 150
    end

    it 'should stop at the target position' do
      expect(curve.advance(15).get).to be 200
    end
  end

  context 'when moving backward' do
    let :curve do
      Linear.new(100, speed: 20).target 0
    end

    it 'should advance with time' do
      expect(curve.advance(2).get).to be 60
    end

    it 'should stop at the target position' do
      expect(curve.advance(10).get).to be 0
    end
  end
end

describe Quadratic do
  let :curve do
    Quadratic.new 0
  end

  it 'should start with the given value' do
    expect(curve.get).to be 0
  end

  it 'should stay constant over time' do
    expect(curve.advance(5).get).to be 0
  end

  context 'when moving forward' do
    let :curve do
      Quadratic.new(100, acceleration: 10).target 140
    end

    it 'should advance with time' do
      expect(curve.advance(1).get).to be 105
    end

    it 'should accelerate over time' do
      expect(curve.advance(1).advance(1).get).to be 120
      expect(curve.advance(2).get).to be 120
    end

    xit 'should decelerate before the target' do
      expect(curve.advance(3).get).to be 135
    end
  end

  context 'when moving backward' do
    let :curve do
      Quadratic.new(100, acceleration: 10).target 60
    end

    it 'should advance with time' do
      expect(curve.advance(1).get).to be 95
    end
  end
end
